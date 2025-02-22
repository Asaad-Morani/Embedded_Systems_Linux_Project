#include "servoMotor.h"
#include "cmsis_os.h"
#include <stdio.h>

extern TIM_HandleTypeDef SERVO_TIMER;

/**
 * @brief  Initializes the Servo-motor PWM
 */
void Servo_Init(void) {
    HAL_TIM_PWM_Start(&SERVO_TIMER, SERVO_CHANNEL);
}

/**
 * @brief  Sets the servo-motor angle
 * @param  angle
 */
void Servo_SetAngle(uint8_t angle) {
    if (angle > SERVO_MAX_ANGLE) {
    	angle = SERVO_MAX_ANGLE; // Limit max angle
    }

    // Convert angle to pulse width 1 to 2 milliseconds
    uint32_t pulse_width = SERVO_MIN_PULSE + ((SERVO_MAX_PULSE - SERVO_MIN_PULSE) * angle) / 180;

    // Convert pulse width from microseconds to timer ticks
    uint32_t pwm_value = (pulse_width * (__HAL_TIM_GET_AUTORELOAD(&SERVO_TIMER) + 1)) / 20000;

    // Set the PWM duty cycle
    __HAL_TIM_SET_COMPARE(&SERVO_TIMER, SERVO_CHANNEL, pwm_value);
}

/**
 * @brief  Task for rotating the servo-motor
 */
void Task_ServoMotor_Handler(void *argument) {
    Servo_Init();

    while (1) {
        // Slowly move from 0 to 180 degrees
        for (uint8_t angle = 0; angle <= 180; angle += 2) {
            Servo_SetAngle(angle);
            osDelay(20);  // Small delay for smooth movement
        }

        // Slowly move back from 180 to 0 degrees
        for (uint8_t angle = 180; angle >= 0; angle -= 2) {
            Servo_SetAngle(angle);
            osDelay(20);  // Small delay for smooth movement
        }
    }
}
