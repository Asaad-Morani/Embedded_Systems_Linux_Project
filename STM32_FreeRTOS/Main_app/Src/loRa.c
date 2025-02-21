#include "loRa.h"
#include "stm32f7xx_hal.h"


// Select LoRa Module (Chip Select LOW)
static void LoRa_Select(void) {
    HAL_GPIO_WritePin(LORA_NSS_PORT, LORA_NSS_PIN, GPIO_PIN_RESET);
    HAL_Delay(5);  // Small delay for stability
}

// Unselect LoRa Module (Chip Select HIGH)
static void LoRa_Unselect(void) {
    HAL_GPIO_WritePin(LORA_NSS_PORT, LORA_NSS_PIN, GPIO_PIN_SET);
    HAL_Delay(5);  // Small delay for stability
}

// Reset LoRa Module
void LoRa_Reset(void) {
    HAL_GPIO_WritePin(LORA_RESET_PORT, LORA_RESET_PIN, GPIO_PIN_RESET);  // Pull RESET low
    HAL_Delay(1);  // Keep it low for 1 ms
    HAL_GPIO_WritePin(LORA_RESET_PORT, LORA_RESET_PIN, GPIO_PIN_SET);  // Pull RESET high
    HAL_Delay(100);  // Wait for the module to initialize
}

// Write to a LoRa register
void LoRa_WriteRegister(uint8_t reg, uint8_t value) {
    LoRa_Select();  // Pull NSS low
    uint8_t data[2] = {reg | 0x80, value};  // MSB set for write
    HAL_SPI_Transmit(&hspi1, data, 2, HAL_MAX_DELAY);
    LoRa_Unselect();  // Pull NSS high
    HAL_Delay(1);  // Short delay to stabilize SPI
}

void LoRa_SetFrequency(uint32_t frequency) {
    uint64_t frf = ((uint64_t)frequency << 19) / 32000000;
    LoRa_WriteRegister(REG_FRF_MSB, (uint8_t)(frf >> 16));
    LoRa_WriteRegister(REG_FRF_MID, (uint8_t)(frf >> 8));
    LoRa_WriteRegister(REG_FRF_LSB, (uint8_t)(frf >> 0));
}

void LoRa_SetTxPower(uint8_t power) {
    if (power > 17) power = 17; // LoRa module max TX power is 17 dBm
    if (power < 2) power = 2;   // Minimum TX power is 2 dBm

    // Configure PA_BOOST (0x80 enables PA_BOOST, lower 4 bits control power level)
    LoRa_WriteRegister(REG_PA_CONFIG, 0x80 | (power - 2));
}

// Read from a LoRa register
uint8_t LoRa_ReadRegister(uint8_t reg) {
    LoRa_Select();  // Pull NSS low
    uint8_t data[2] = {reg & 0x7F, 0x00};  // MSB cleared for read
    uint8_t received[2] = {0};
    HAL_SPI_TransmitReceive(&hspi1, data, received, 2, HAL_MAX_DELAY);
    LoRa_Unselect();  // Pull NSS high
    return received[1];
}

void LoRa_Init(void) {
    LoRa_Reset();

    // Set LoRa to sleep mode
    LoRa_WriteRegister(REG_OP_MODE, MODE_SLEEP | MODE_LONG_RANGE_MODE);
    HAL_Delay(10);

    // Set frequency
    LoRa_SetFrequency(LORA_FREQ);

    // Set TX power
    LoRa_SetTxPower(17);

    // Configure registers
    LoRa_WriteRegister(REG_LNA, 0x23);
    LoRa_WriteRegister(REG_FIFO_TX_BASE_ADDR, 0x80);
    LoRa_WriteRegister(REG_FIFO_RX_BASE_ADDR, 0x00);
    LoRa_WriteRegister(REG_MODEM_CONFIG_1, 0x72);
    LoRa_WriteRegister(REG_MODEM_CONFIG_2, 0x74);
    LoRa_WriteRegister(REG_SYMB_TIMEOUT_LSB, 0x08);
    LoRa_WriteRegister(REG_PAYLOAD_LENGTH, 0x40);

    // Read back registers for debugging
    uint8_t mode = LoRa_ReadRegister(REG_OP_MODE);
    uint8_t frf_msb = LoRa_ReadRegister(REG_FRF_MSB);
    uint8_t frf_mid = LoRa_ReadRegister(REG_FRF_MID);
    uint8_t frf_lsb = LoRa_ReadRegister(REG_FRF_LSB);
    uint8_t pa_config = LoRa_ReadRegister(REG_PA_CONFIG);

    printf("\r LoRa Mode After Init: 0x%02X\n", mode);
    printf("\r LoRa Frequency: 0x%02X 0x%02X 0x%02X\n", frf_msb, frf_mid, frf_lsb);
    printf("\r LoRa PA Config: 0x%02X\n", pa_config);

}

void LoRa_SendPacket(uint8_t *data, uint8_t size) {
    LoRa_WriteRegister(REG_OP_MODE, MODE_STDBY | MODE_LONG_RANGE_MODE);
    LoRa_WriteRegister(REG_FIFO_ADDR_PTR, 0x80);

    // Step 1: Send the length as the first byte
    LoRa_WriteRegister(REG_FIFO, size);

    // Step 2: Send actual data
    for (uint8_t i = 0; i < size; i++) {
        LoRa_WriteRegister(REG_FIFO, data[i]);
    }

    // Step 3: Set payload length
    LoRa_WriteRegister(REG_PAYLOAD_LENGTH, size + 1);  // +1 for the length byte

    // Step 4: Transmit
    LoRa_WriteRegister(REG_OP_MODE, MODE_TX | MODE_LONG_RANGE_MODE);

    // Read IRQ flags before TX
	uint8_t irq_flags_before = LoRa_ReadRegister(REG_IRQ_FLAGS);
	//printf("\r LoRa TX IRQ Flags Before: 0x%02X\n", irq_flags_before);

    // Wait for TX done (should set bit 3 = 1)
	while ((LoRa_ReadRegister(REG_IRQ_FLAGS) & 0x08) == 0) {
	   //printf("Waiting... IRQ Flags: 0x%02X\n", LoRa_ReadRegister(REG_IRQ_FLAGS));
	   HAL_Delay(100);  // Small delay to avoid spamming output
	}

    // Read IRQ flags after TX
    uint8_t irq_flags_after = LoRa_ReadRegister(REG_IRQ_FLAGS);
    //printf("\r LoRa TX IRQ Flags After: 0x%02X\n", irq_flags_after);

    // Clear IRQ flags
    LoRa_WriteRegister(REG_IRQ_FLAGS, 0x08);
}

void Task_LoRa_Handler(void *argument) {
	//printf("\r Initializing LoRa Transmitter...\n");
	int c=0;
    LoRa_Init();
    //printf("\r LoRa Transmitter Initialized.\n");

    uint8_t txBuffer[32];

    uint8_t version = LoRa_ReadRegister(0x42);
    //printf("\r _______________ LoRa Version: 0x%02X\n", version);

    for (;;) {
    	//printf("\r iteration: %d \n", c);
    	c++;
        snprintf((char *)txBuffer, sizeof(txBuffer), "Hello from STM32");

        uint8_t mode = LoRa_ReadRegister(REG_OP_MODE);
        //printf("\r LoRa Mode Before TX: 0x%02X\n", mode);

        LoRa_WriteRegister(REG_IRQ_FLAGS, 0xFF); // Clear all IRQ flags before sending

        // Set LoRa to TX mode
        LoRa_SendPacket(txBuffer, strlen((char *)txBuffer));
        //printf("\r Sent: %s\n", txBuffer); // Debug message

        mode = LoRa_ReadRegister(REG_OP_MODE);
        //printf("\r LoRa Mode After TX: 0x%02X\n", mode);

        // Read IRQ flags after sending
        uint8_t irq_flags = LoRa_ReadRegister(REG_IRQ_FLAGS);
        //printf("\r LoRa TX IRQ Flags: 0x%02X\n", irq_flags);

        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);  // Toggle Blue LED

        //printf("\r __________________________________ \n");

        osDelay(1000);
    }
}
