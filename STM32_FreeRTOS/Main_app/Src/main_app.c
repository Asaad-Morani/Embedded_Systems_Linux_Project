/*
 * main_app.c
 *
 *  Created on: Jan 15, 2025
 *      Author: asaad
 */

#include "main_app.h"
#include "ultra_sonic.h"

// Task implementation
void main_application(void *argument) {

	// Start Ultrasonic Sensor Task (optional coordination logic can go here)
	printf("\r Main application started.\n");
	for (;;) {
		printf("\rIm inside main app \n");
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);  // Toggle another LED (for example)
		osDelay(500);  // 1-second delay
	}
}
