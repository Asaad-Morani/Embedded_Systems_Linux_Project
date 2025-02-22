#ifndef INC_SERVOMOTOR_H_
#define INC_SERVOMOTOR_H_

#include "stm32f7xx_hal.h"

/* Define Servo-Motor PWM Parameters */
#define SERVO_TIMER        htim3
#define SERVO_CHANNEL      TIM_CHANNEL_1
#define SERVO_MIN_PULSE    1000           // Min pulse width in microseconds (0 degrees)
#define SERVO_MAX_PULSE    2000           // Max pulse width in microseconds (180 degrees)
#define SERVO_MAX_ANGLE    180            // Max angle of the servo-motor

/* Function Prototypes */
void Servo_Init(void);
void Servo_SetAngle(uint8_t angle);
void Task_ServoMotor_Handler(void *argument);

#endif /* INC_SERVOMOTOR_H_ */
