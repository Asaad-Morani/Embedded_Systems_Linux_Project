#ifndef INC_OLED_H_
#define INC_OLED_H_

#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_i2c.h"
#include "FreeRTOS.h"
#include "queue.h"
#include <string.h>


extern QueueHandle_t distanceQueue;  // Declare the queue as external
extern TIM_HandleTypeDef hi2c1;  // Declare htim2 as external

// Function prototypes
void OLED_Init(void);
void OLED_Clear(void);
void Task_OLED_Handler(void *argument);

#endif /* INC_OLED_H_ */
