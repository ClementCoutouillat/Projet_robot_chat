################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/mylibs/shell/drv_uart.c \
../Core/mylibs/shell/shell.c \
../Core/mylibs/shell/shell_founction.c 

OBJS += \
./Core/mylibs/shell/drv_uart.o \
./Core/mylibs/shell/shell.o \
./Core/mylibs/shell/shell_founction.o 

C_DEPS += \
./Core/mylibs/shell/drv_uart.d \
./Core/mylibs/shell/shell.d \
./Core/mylibs/shell/shell_founction.d 


# Each subdirectory must supply rules for building sources it contributes
Core/mylibs/shell/%.o Core/mylibs/shell/%.su Core/mylibs/shell/%.cyclo: ../Core/mylibs/shell/%.c Core/mylibs/shell/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G070xx -c -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0 -I"D:/ENSEA/Coursenensea/annee3/ESE/Systemes_a_microcontroleurs/TP/Projet_robot_chat/robot_main/Core/mylibs/board" -I"D:/ENSEA/Coursenensea/annee3/ESE/Systemes_a_microcontroleurs/TP/Projet_robot_chat/robot_main/Core/mylibs/bumper" -I"D:/ENSEA/Coursenensea/annee3/ESE/Systemes_a_microcontroleurs/TP/Projet_robot_chat/robot_main/Core/mylibs/led" -I"D:/ENSEA/Coursenensea/annee3/ESE/Systemes_a_microcontroleurs/TP/Projet_robot_chat/robot_main/Core/mylibs/motor" -I"D:/ENSEA/Coursenensea/annee3/ESE/Systemes_a_microcontroleurs/TP/Projet_robot_chat/robot_main/Core/mylibs/shell" -I"D:/ENSEA/Coursenensea/annee3/ESE/Systemes_a_microcontroleurs/TP/Projet_robot_chat/robot_main/Core/mylibs/ydlidar" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-mylibs-2f-shell

clean-Core-2f-mylibs-2f-shell:
	-$(RM) ./Core/mylibs/shell/drv_uart.cyclo ./Core/mylibs/shell/drv_uart.d ./Core/mylibs/shell/drv_uart.o ./Core/mylibs/shell/drv_uart.su ./Core/mylibs/shell/shell.cyclo ./Core/mylibs/shell/shell.d ./Core/mylibs/shell/shell.o ./Core/mylibs/shell/shell.su ./Core/mylibs/shell/shell_founction.cyclo ./Core/mylibs/shell/shell_founction.d ./Core/mylibs/shell/shell_founction.o ./Core/mylibs/shell/shell_founction.su

.PHONY: clean-Core-2f-mylibs-2f-shell

