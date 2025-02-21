#ifndef INC_TOOLS_
#define INC_TOOLS_

#include "stm32f7xx_hal.h"

/** @brief Define the UART debug port */
extern UART_HandleTypeDef huart3;
#define DEBUG_UART &huart3

#define LINE_FEED 10
#define CARRIAGE_RETURN 13

int _read(int file, char *ptr, int len);
int __io_putchar(int ch);
int _write(int file, char *ptr, int len);
int __io_getchar(void);
int _read(int file, char *ptr, int len);


#endif /* INC_TOOLS_ */
