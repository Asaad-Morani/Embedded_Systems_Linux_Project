#ifndef INC_ULTRA_SONIC_H_
#define INC_ULTRA_SONIC_H_

#include "stm32f7xx_hal.h"
#include "FreeRTOS.h"
#include "queue.h"

extern QueueHandle_t distanceQueue;  // Declare the queue as external
extern TIM_HandleTypeDef htim1;  // Declare htim2 as external

// Define GPIO ports and pins for HC-SR04
#define ULTRA_SONIC_TRIG_PORT GPIOB
#define ULTRA_SONIC_TRIG_PIN  GPIO_PIN_1
#define ULTRA_SONIC_ECHO_PORT GPIOB
#define ULTRA_SONIC_ECHO_PIN  GPIO_PIN_2

// Function prototypes
void UltraSonic_Trigger(void);      // Send a 10 µs pulse on TRIG pin
uint32_t UltraSonic_GetDistance(void); // Measure distance in cm
void Task_Ultrasonic_Handler(void *argument); // Task implementation

#endif /* INC_ULTRA_SONIC_H_ */
