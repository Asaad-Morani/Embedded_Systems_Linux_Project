#ifndef INC_GPS_H_
#define INC_GPS_H_

#include "stm32f7xx_hal.h"
#include <stdint.h>
#include <stdbool.h>

extern UART_HandleTypeDef huart5;  // GPS is connected to UART5

/* Define buffer sizes */
#define GPS_RX_BUFFER_SIZE 128
#define GPS_MAX_NMEA_LENGTH 82  // Standard NMEA max length

/* GPS Data Structure */
typedef struct {
    float decimalLat;
    float decimalLong;
    float utcTime;
    char northsouth;
    char eastwest;
    char posStatus;
} GPS_Data_t;

extern GPS_Data_t gps_data; // Global GPS data struct

/* Function prototypes */
float nmea_To_Decimal(float coordinate);
void Gps_Parse(char *strParse);
int Gps_Validate(char *nmea);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void GPS_Init();
void Task_GPS_Handler();

#endif /* INC_GPS_H_ */
