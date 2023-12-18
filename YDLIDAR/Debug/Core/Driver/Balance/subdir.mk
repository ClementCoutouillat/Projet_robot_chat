################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Driver/Balance/avoidObstacle.c \
../Core/Driver/Balance/filter.c \
../Core/Driver/Balance/system.c 

OBJS += \
./Core/Driver/Balance/avoidObstacle.o \
./Core/Driver/Balance/filter.o \
./Core/Driver/Balance/system.o 

C_DEPS += \
./Core/Driver/Balance/avoidObstacle.d \
./Core/Driver/Balance/filter.d \
./Core/Driver/Balance/system.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Driver/Balance/%.o Core/Driver/Balance/%.su Core/Driver/Balance/%.cyclo: ../Core/Driver/Balance/%.c Core/Driver/Balance/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F410Rx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/ENSEA/Coursenensea/annee3/ESE/Systemes_a_microcontroleurs/TP/Projet_robot_chat/YDLIDAR/Core/Driver/Ydliar" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Driver-2f-Balance

clean-Core-2f-Driver-2f-Balance:
	-$(RM) ./Core/Driver/Balance/avoidObstacle.cyclo ./Core/Driver/Balance/avoidObstacle.d ./Core/Driver/Balance/avoidObstacle.o ./Core/Driver/Balance/avoidObstacle.su ./Core/Driver/Balance/filter.cyclo ./Core/Driver/Balance/filter.d ./Core/Driver/Balance/filter.o ./Core/Driver/Balance/filter.su ./Core/Driver/Balance/system.cyclo ./Core/Driver/Balance/system.d ./Core/Driver/Balance/system.o ./Core/Driver/Balance/system.su

.PHONY: clean-Core-2f-Driver-2f-Balance

