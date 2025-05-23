################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/DWT_Delay.c \
../Core/Src/I2C_LCD.c \
../Core/Src/I2C_LCD_cfg.c \
../Core/Src/KEYPAD.c \
../Core/Src/KEYPAD_cfg.c \
../Core/Src/SERVO.c \
../Core/Src/SERVO_cfg.c \
../Core/Src/Timer_Delay.c \
../Core/Src/Util.c \
../Core/Src/main.c \
../Core/Src/rc522.c \
../Core/Src/stm32f1xx_hal_msp.c \
../Core/Src/stm32f1xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f1xx.c \
../Core/Src/uart.c 

OBJS += \
./Core/Src/DWT_Delay.o \
./Core/Src/I2C_LCD.o \
./Core/Src/I2C_LCD_cfg.o \
./Core/Src/KEYPAD.o \
./Core/Src/KEYPAD_cfg.o \
./Core/Src/SERVO.o \
./Core/Src/SERVO_cfg.o \
./Core/Src/Timer_Delay.o \
./Core/Src/Util.o \
./Core/Src/main.o \
./Core/Src/rc522.o \
./Core/Src/stm32f1xx_hal_msp.o \
./Core/Src/stm32f1xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f1xx.o \
./Core/Src/uart.o 

C_DEPS += \
./Core/Src/DWT_Delay.d \
./Core/Src/I2C_LCD.d \
./Core/Src/I2C_LCD_cfg.d \
./Core/Src/KEYPAD.d \
./Core/Src/KEYPAD_cfg.d \
./Core/Src/SERVO.d \
./Core/Src/SERVO_cfg.d \
./Core/Src/Timer_Delay.d \
./Core/Src/Util.d \
./Core/Src/main.d \
./Core/Src/rc522.d \
./Core/Src/stm32f1xx_hal_msp.d \
./Core/Src/stm32f1xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f1xx.d \
./Core/Src/uart.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/DWT_Delay.cyclo ./Core/Src/DWT_Delay.d ./Core/Src/DWT_Delay.o ./Core/Src/DWT_Delay.su ./Core/Src/I2C_LCD.cyclo ./Core/Src/I2C_LCD.d ./Core/Src/I2C_LCD.o ./Core/Src/I2C_LCD.su ./Core/Src/I2C_LCD_cfg.cyclo ./Core/Src/I2C_LCD_cfg.d ./Core/Src/I2C_LCD_cfg.o ./Core/Src/I2C_LCD_cfg.su ./Core/Src/KEYPAD.cyclo ./Core/Src/KEYPAD.d ./Core/Src/KEYPAD.o ./Core/Src/KEYPAD.su ./Core/Src/KEYPAD_cfg.cyclo ./Core/Src/KEYPAD_cfg.d ./Core/Src/KEYPAD_cfg.o ./Core/Src/KEYPAD_cfg.su ./Core/Src/SERVO.cyclo ./Core/Src/SERVO.d ./Core/Src/SERVO.o ./Core/Src/SERVO.su ./Core/Src/SERVO_cfg.cyclo ./Core/Src/SERVO_cfg.d ./Core/Src/SERVO_cfg.o ./Core/Src/SERVO_cfg.su ./Core/Src/Timer_Delay.cyclo ./Core/Src/Timer_Delay.d ./Core/Src/Timer_Delay.o ./Core/Src/Timer_Delay.su ./Core/Src/Util.cyclo ./Core/Src/Util.d ./Core/Src/Util.o ./Core/Src/Util.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/rc522.cyclo ./Core/Src/rc522.d ./Core/Src/rc522.o ./Core/Src/rc522.su ./Core/Src/stm32f1xx_hal_msp.cyclo ./Core/Src/stm32f1xx_hal_msp.d ./Core/Src/stm32f1xx_hal_msp.o ./Core/Src/stm32f1xx_hal_msp.su ./Core/Src/stm32f1xx_it.cyclo ./Core/Src/stm32f1xx_it.d ./Core/Src/stm32f1xx_it.o ./Core/Src/stm32f1xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f1xx.cyclo ./Core/Src/system_stm32f1xx.d ./Core/Src/system_stm32f1xx.o ./Core/Src/system_stm32f1xx.su ./Core/Src/uart.cyclo ./Core/Src/uart.d ./Core/Src/uart.o ./Core/Src/uart.su

.PHONY: clean-Core-2f-Src

