#ifndef INC_LORA_H_
#define INC_LORA_H_


#include "stm32f7xx_hal.h"
#include <stdint.h>

extern SPI_HandleTypeDef hspi1;  // Declare the SPI1 handle


// LoRa module settings
#define LORA_SPI_HANDLE &hspi1         // Change this to the SPI handle you're using
#define LORA_NSS_PORT GPIOA            // NSS GPIO Port
#define LORA_NSS_PIN GPIO_PIN_4        // NSS GPIO Pin
#define LORA_RESET_PORT GPIOB          // Reset GPIO Port
#define LORA_RESET_PIN GPIO_PIN_4      // Reset GPIO Pin
#define LORA_FREQ 433000000            // Frequency in Hz (e.g., 433 MHz)

// LoRa register addresses
#define REG_FIFO 0x00
#define REG_OP_MODE 0x01
#define REG_FRF_MSB 0x06
#define REG_FRF_MID 0x07
#define REG_FRF_LSB 0x08
#define REG_LNA 0x0C
#define REG_FIFO_ADDR_PTR 0x0D
#define REG_FIFO_TX_BASE_ADDR 0x0E
#define REG_FIFO_RX_BASE_ADDR 0x0F
#define REG_MODEM_CONFIG_1 0x1D
#define REG_MODEM_CONFIG_2 0x1E
#define REG_SYMB_TIMEOUT_LSB 0x1F
#define REG_PAYLOAD_LENGTH 0x22
#define REG_IRQ_FLAGS 0x12
#define REG_PA_CONFIG 0x09

#define MODE_SLEEP 0x00
#define MODE_STDBY 0x01
#define MODE_TX 0x03
#define MODE_RXCONTINUOUS 0x05
#define MODE_LONG_RANGE_MODE 0x80

void LoRa_Init(void);
void LoRa_SetFrequency(uint32_t frequency);
void LoRa_SetTxPower(uint8_t power);
void LoRa_SendPacket(uint8_t *data, uint8_t size);
void Task_LoRa_Handler(void *argument);


#endif /* INC_LORA_H_ */
