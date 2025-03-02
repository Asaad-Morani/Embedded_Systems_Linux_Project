#include "gps.h"
#include <string.h>
#include <stdio.h>
#include "cmsis_os.h"

/* Global Variables */
GPS_Data_t gps_data;
uint8_t gps_rx_buffer[GPS_RX_BUFFER_SIZE] = {0};
uint8_t gps_rx_index = 0;
uint8_t gps_rx_data;
float latitude = 0.0;
float longitude = 0.0;

/**
 * @brief Convert NMEA coordinate format to decimal degrees
 * @param coordinate NMEA format coordinate
 * @return Converted decimal coordinate
 */
float nmea_To_Decimal(float coordinate) {
    int degree = (int)(coordinate / 100);
    float minutes = coordinate - (degree * 100);
    return degree + (minutes / 60);
}

/**
 * @brief Parse GPS sentences and extract latitude and longitude
 * @param strParse NMEA sentence string
 */
void Gps_Parse(char *strParse) {
    if (strncmp(strParse, "$GPRMC", 6) == 0) {
        char status;
        sscanf(strParse, "$GPRMC,%*f,%c,%f,%c,%f,%c",
               &status, &gps_data.decimalLat, &gps_data.northsouth,
               &gps_data.decimalLong, &gps_data.eastwest);

        if (status == 'A') {  // Process only valid fixes
            latitude = nmea_To_Decimal(gps_data.decimalLat);
            longitude = nmea_To_Decimal(gps_data.decimalLong);
        }
    }
}

/**
 * @brief Validate NMEA Checksum
 * @param nmea NMEA sentence string
 * @return 1 if valid, 0 otherwise
 */
int Gps_Validate(char *nmea) {
    char check[3];
    char calculatedString[3];
    int index = 0;
    int calculatedCheck = 0;

    if (nmea[index] == '$') index++; else return 0;

    while ((nmea[index] != 0) && (nmea[index] != '*') && (index < 75)) {
        calculatedCheck ^= nmea[index];
        index++;
    }

    if (index >= 75) return 0;
    if (nmea[index] == '*') {
        check[0] = nmea[index + 1];
        check[1] = nmea[index + 2];
        check[2] = 0;
    } else return 0;

    sprintf(calculatedString, "%02X", calculatedCheck);
    return ((calculatedString[0] == check[0]) && (calculatedString[1] == check[1])) ? 1 : 0;
}

/**
 * @brief UART RX Interrupt Callback for handling received GPS data
 * @param huart UART handle
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == UART5) {
        gps_rx_data = (uint8_t)(huart5.Instance->RDR & 0xFF);

        if (gps_rx_index < sizeof(gps_rx_buffer) - 1) {
            gps_rx_buffer[gps_rx_index++] = gps_rx_data;

            if (gps_rx_data == '\n') {
                gps_rx_buffer[gps_rx_index] = '\0';

                if (gps_rx_buffer[0] == '$') {
                    if (Gps_Validate((char *)gps_rx_buffer)) {
                    	Gps_Parse((char *)gps_rx_buffer);
                    }
                }

                gps_rx_index = 0;  // Reset buffer
            }
        } else {
            gps_rx_index = 0;  // Prevent buffer overflow
        }

        // Restart UART reception
        HAL_UART_Receive_IT(&huart5, &gps_rx_data, 1);
    }
}

/**
 * @brief Initialize GPS UART
 */
void GPS_Init() {
    HAL_UART_Receive_IT(&huart5, &gps_rx_data, 1);
}

/**
 * @brief FreeRTOS task to handle GPS data
 */
void Task_GPS_Handler() {
    GPS_Init();
    for (;;) {
        if (latitude != 0.0 && longitude != 0.0) {
            printf("\r %.6f N %.6f E\n", latitude, longitude);
        }
        osDelay(1000);
    }
}
