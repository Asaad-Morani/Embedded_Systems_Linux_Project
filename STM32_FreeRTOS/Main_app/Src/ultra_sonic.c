#include "ultra_sonic.h"

// Send 10 µs pulse on TRIG pin
void UltraSonic_Trigger(void) {
    HAL_GPIO_WritePin(ULTRA_SONIC_TRIG_PORT, ULTRA_SONIC_TRIG_PIN, GPIO_PIN_SET);  // TRIG HIGH
    HAL_Delay(1);  // This generates a ~1 ms pulse, not 10 µs
    HAL_GPIO_WritePin(ULTRA_SONIC_TRIG_PORT, ULTRA_SONIC_TRIG_PIN, GPIO_PIN_RESET);  // TRIG LOW
}

// Measure the duration of the ECHO pulse and calculate distance
uint32_t UltraSonic_GetDistance(void) {
    uint32_t startTime = 0, endTime = 0;
    uint32_t pulseDuration = 0;
    uint32_t midTime = 0;
    uint32_t loopCount = 0;

    // Trigger the sensor
    UltraSonic_Trigger();
    //printf("\r Triggered sensor\n");

    __HAL_TIM_SET_COUNTER(&htim1,0);

    // Wait for ECHO pin to go HIGH
    while (HAL_GPIO_ReadPin(ULTRA_SONIC_ECHO_PORT, ULTRA_SONIC_ECHO_PIN) == GPIO_PIN_RESET);

    startTime = __HAL_TIM_GET_COUNTER(&htim1);

    // Wait for ECHO pin to go LOW
    while (HAL_GPIO_ReadPin(ULTRA_SONIC_ECHO_PORT, ULTRA_SONIC_ECHO_PIN) == GPIO_PIN_SET){
    	midTime = __HAL_TIM_GET_COUNTER(&htim1);
    	loopCount++;
    }
    endTime = __HAL_TIM_GET_COUNTER(&htim1);


    // Handle timer overflow
    if (endTime >= startTime) {
        pulseDuration = endTime - startTime;  // Normal case
        //printf("\r No overflow time\n");
    } else {
        pulseDuration = ((0xFFFF - startTime) + endTime + 1);  // Overflow case
        //printf("\r Overflow time\n");
    }

    // Convert to distance
    uint32_t distance = pulseDuration / 58.0;
    //printf("\r Pulse Duration: %lu µs, Distance: %lu cm\n", pulseDuration, distance);

    // Filter out invalid distances
    if (distance > 400) {
        //printf("\r Invalid distance: %lu cm (out of range)\n", distance);
        return 0;  // Return 0 for invalid measurement
    }

//    printf("\r ECHO HIGH detected, startTime: %lu\n", startTime);
//    printf("\r ECHO LOW detected, endTime: %lu\n", endTime);
//    printf("\r ECHO LOW detected, midTime: %lu\n", midTime);
//    printf("\r Loop executed %lu times\n", loopCount);

    return distance;
}

// Task implementation
void Task_Ultrasonic_Handler(void *argument) {
	HAL_TIM_Base_Start(&htim1);

	uint32_t dist=0.0;
	for (;;) {
		dist = UltraSonic_GetDistance();  // Measure distance
		if (xQueueSend(distanceQueue, &dist, pdMS_TO_TICKS(100)) != pdPASS) {
			printf("\r Queue send failed! Distance: %d\n", dist);
		} else {
			printf("\r Distance sent to queue: %d\n", dist);
		}

		// Check the available queue spaces
		UBaseType_t availableSpace = uxQueueSpacesAvailable(distanceQueue);
		printf("\r Queue spaces available: %d\n", availableSpace);

		osDelay(500);  // Adjust as necessary
	}
}


