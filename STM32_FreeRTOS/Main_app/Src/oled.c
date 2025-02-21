#include "oled.h"

#define OLED_I2C_ADDR 0x78  // Adjust if your OLED has a different address

// SSD1306 Command Set
#define OLED_CMD 0x00  // Command mode (The second byte in the buffer contains the actual command to be executed by the OLED)
#define OLED_DATA 0x40 // Data mode (This byte tells the OLED controller that the next byte is a command, not pixel data)

// Basic font (only supports '0', '1', '2')
const uint8_t BasicFont[10][7] = {
	{0x3E, 0x51, 0x49, 0x45, 0x3E}, // '0'
	{0x00, 0x42, 0x7F, 0x40, 0x00}, // '1'
	{0x42, 0x61, 0x51, 0x49, 0x46}, // '2'
	{0x21, 0x41, 0x45, 0x4B, 0x31}, // '3'
	{0x18, 0x14, 0x12, 0x7F, 0x10}, // '4'
	{0x27, 0x45, 0x45, 0x45, 0x39}, // '5'
	{0x3C, 0x4A, 0x49, 0x49, 0x30}, // '6'
	{0x01, 0x71, 0x09, 0x05, 0x03}, // '7'
	{0x36, 0x49, 0x49, 0x49, 0x36}, // '8'
	{0x06, 0x49, 0x49, 0x29, 0x1E}  // '9'
};

void OLED_SendCommand(uint8_t command) {
    uint8_t data[2];                     // Temporary buffer to hold the command
    data[0] = OLED_CMD;                  // First byte specifies "Command mode"
    data[1] = command;                   // Second byte is the actual command
    HAL_I2C_Master_Transmit(             // Send the command via I2C
        &hi2c1,                          // I2C handle (configured in STM32)
        OLED_I2C_ADDR,                   // OLED's I2C address
        data,                            // Pointer to the command buffer
        2,                               // Total size of the buffer (2 bytes)
        HAL_MAX_DELAY                    // Timeout duration
    );
}


void OLED_SendData(uint8_t *data, size_t size) {
    uint8_t buffer[size + 1];               // Temporary buffer
    buffer[0] = OLED_DATA;                  // First byte specifies "Data mode"
    memcpy(&buffer[1], data, size);         // Copy pixel data into the buffer
    HAL_I2C_Master_Transmit(                // Send the buffer via I2C
        &hi2c1,                             // I2C handle (configured in STM32)
        OLED_I2C_ADDR,                      // OLED's I2C address
        buffer,                             // Pointer to the buffer
        size + 1,                           // Total size of the buffer
        HAL_MAX_DELAY                       // Timeout duration
    );
}

void OLED_Init(void) {
    HAL_Delay(100);  // Wait for the screen to power up

    // Initialization sequence for SSD1306
    OLED_SendCommand(0xAE); // Display OFF
    OLED_SendCommand(0x20); // Set Memory Addressing Mode
    OLED_SendCommand(0x10); // Horizontal Addressing Mode
    OLED_SendCommand(0xB0); // Set Page Start Address for Page Addressing Mode
    OLED_SendCommand(0xC8); // COM Output Scan Direction
    OLED_SendCommand(0x00); // Set Low Column Address
    OLED_SendCommand(0x10); // Set High Column Address
    OLED_SendCommand(0x40); // Set Start Line Address
    OLED_SendCommand(0x81); // Set Contrast Control
    OLED_SendCommand(0xFF); // Max contrast
    OLED_SendCommand(0xA1); // Set Segment Re-map
    OLED_SendCommand(0xA6); // Set Normal/Inverse Display
    OLED_SendCommand(0xA8); // Set Multiplex Ratio
    OLED_SendCommand(0x3F); // 1/64 Duty
    OLED_SendCommand(0xA4); // Entire Display ON
    OLED_SendCommand(0xD3); // Set Display Offset
    OLED_SendCommand(0x00); // No Offset
    OLED_SendCommand(0xD5); // Set Display Clock Divide Ratio
    OLED_SendCommand(0xF0); // Max frequency
    OLED_SendCommand(0xD9); // Set Pre-charge Period
    OLED_SendCommand(0x22); // Default
    OLED_SendCommand(0xDA); // Set COM Pins Hardware Configuration
    OLED_SendCommand(0x12); //
    OLED_SendCommand(0xDB); // Set VCOMH Deselect Level
    OLED_SendCommand(0x20); // Default
    OLED_SendCommand(0x8D); // Enable Charge Pump
    OLED_SendCommand(0x14); //
    OLED_SendCommand(0xAF); // Display ON
}

void OLED_Clear(void) {
    uint8_t clear_data[128] = {0}; // One line of empty pixels (128x0)
    for (int i = 0; i < 8; i++) {  // 8 pages for 64-pixel height
        OLED_SendCommand(0xB0 + i); // Set page address
        OLED_SendCommand(0x00);    // Set lower column start address
        OLED_SendCommand(0x10);    // Set higher column start address
        OLED_SendData(clear_data, 128);
    }
}

// Set cursor position
void OLED_SetCursor(uint8_t x, uint8_t y) {
    OLED_SendCommand(0xB0 + y);                 // Set page address
    OLED_SendCommand(0x00 + (x & 0x0F));        // Set lower column address
    OLED_SendCommand(0x10 + ((x >> 4) & 0x0F)); // Set higher column address
}

// Write a string to the OLED
void OLED_WriteString(char *str) {
	uint8_t cursorX = 0;
	uint8_t cursorY = 0;

    while (*str) {
        const uint8_t *char_data = NULL;

        // Map characters to font array (for digits only)
		if (*str >= '0' && *str <= '9') {
			char_data = BasicFont[*str - '0'];
		}

		// Write character if valid
		if (char_data) {
			OLED_SetCursor(cursorX, cursorY);
			OLED_SendData((uint8_t *)char_data, 5);  // Assuming 5 bytes per character
			cursorX += 6;  // Adjust spacing (character width + 1)
		}

        str++;
    }
}

void Task_OLED_Handler(void *argument) {
    uint32_t distance = 0;
    char buffer[16];

    OLED_Init();
    OLED_Clear();

    for (;;) {
        // Receive the distance value from the queue with a timeout
        if (xQueueReceive(distanceQueue, &distance, pdMS_TO_TICKS(500)) == pdPASS) {
			snprintf(buffer, sizeof(buffer), "Distance: %lu cm", distance);

			// Debug message to verify distance
			printf("\r Distance from OLED handler: %lu cm \n", distance);
			printf("\r ____________________________ \n");

			// Update the OLED display
			OLED_Clear();
			OLED_SetCursor(0, 0); // Set cursor to the start
			OLED_WriteString(buffer);  // Write the distance string
			//lastDistance = distance;  // Save the last displayed distance
		} else {
            printf("\r No data received from queue within timeout.\n");
        }

        // Optional delay (can be omitted if the task is blocked by xQueueReceive)
        osDelay(50);
    }
}


