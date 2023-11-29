################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Driver/Ydliar/Ydlidar.c 

OBJS += \
./Core/Driver/Ydliar/Ydlidar.o 

C_DEPS += \
./Core/Driver/Ydliar/Ydlidar.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Driver/Ydliar/%.o Core/Driver/Ydliar/%.su Core/Driver/Ydliar/%.cyclo: ../Core/Driver/Ydliar/%.c Core/Driver/Ydliar/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F410Rx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/ENSEA/Coursenensea/annee3/ESE/Systemes_a_microcontroleurs/TP/Projet_robot_chat/YDLIDAR/Core/Driver/Ydliar" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Driver-2f-Ydliar

clean-Core-2f-Driver-2f-Ydliar:
	-$(RM) ./Core/Driver/Ydliar/Ydlidar.cyclo ./Core/Driver/Ydliar/Ydlidar.d ./Core/Driver/Ydliar/Ydlidar.o ./Core/Driver/Ydliar/Ydlidar.su

.PHONY: clean-Core-2f-Driver-2f-Ydliar

