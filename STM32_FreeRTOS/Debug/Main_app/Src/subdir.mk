################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Main_app/Src/gps.c \
../Main_app/Src/loRa.c \
../Main_app/Src/main_app.c \
../Main_app/Src/oled.c \
../Main_app/Src/servoMotor.c \
../Main_app/Src/tools.c \
../Main_app/Src/ultra_sonic.c 

OBJS += \
./Main_app/Src/gps.o \
./Main_app/Src/loRa.o \
./Main_app/Src/main_app.o \
./Main_app/Src/oled.o \
./Main_app/Src/servoMotor.o \
./Main_app/Src/tools.o \
./Main_app/Src/ultra_sonic.o 

C_DEPS += \
./Main_app/Src/gps.d \
./Main_app/Src/loRa.d \
./Main_app/Src/main_app.d \
./Main_app/Src/oled.d \
./Main_app/Src/servoMotor.d \
./Main_app/Src/tools.d \
./Main_app/Src/ultra_sonic.d 


# Each subdirectory must supply rules for building sources it contributes
Main_app/Src/%.o Main_app/Src/%.su Main_app/Src/%.cyclo: ../Main_app/Src/%.c Main_app/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g -DDEBUG -DUSE_HAL_DRIVER -DSTM32F756xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I"/home/asaad/Desktop/Embedded_Systems_Linux_Project/STM32_FreeRTOS/Core/Inc" -I"/home/asaad/Desktop/Embedded_Systems_Linux_Project/STM32_FreeRTOS/Main_app/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Main_app-2f-Src

clean-Main_app-2f-Src:
	-$(RM) ./Main_app/Src/gps.cyclo ./Main_app/Src/gps.d ./Main_app/Src/gps.o ./Main_app/Src/gps.su ./Main_app/Src/loRa.cyclo ./Main_app/Src/loRa.d ./Main_app/Src/loRa.o ./Main_app/Src/loRa.su ./Main_app/Src/main_app.cyclo ./Main_app/Src/main_app.d ./Main_app/Src/main_app.o ./Main_app/Src/main_app.su ./Main_app/Src/oled.cyclo ./Main_app/Src/oled.d ./Main_app/Src/oled.o ./Main_app/Src/oled.su ./Main_app/Src/servoMotor.cyclo ./Main_app/Src/servoMotor.d ./Main_app/Src/servoMotor.o ./Main_app/Src/servoMotor.su ./Main_app/Src/tools.cyclo ./Main_app/Src/tools.d ./Main_app/Src/tools.o ./Main_app/Src/tools.su ./Main_app/Src/ultra_sonic.cyclo ./Main_app/Src/ultra_sonic.d ./Main_app/Src/ultra_sonic.o ./Main_app/Src/ultra_sonic.su

.PHONY: clean-Main_app-2f-Src

