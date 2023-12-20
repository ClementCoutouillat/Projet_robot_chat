################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/mylibs/ydlidar/Ydlidar.c 

OBJS += \
./Core/mylibs/ydlidar/Ydlidar.o 

C_DEPS += \
./Core/mylibs/ydlidar/Ydlidar.d 


# Each subdirectory must supply rules for building sources it contributes
Core/mylibs/ydlidar/%.o Core/mylibs/ydlidar/%.su Core/mylibs/ydlidar/%.cyclo: ../Core/mylibs/ydlidar/%.c Core/mylibs/ydlidar/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G070xx -c -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0 -I"D:/ENSEA/Coursenensea/annee3/ESE/Systemes_a_microcontroleurs/TP/Projet_robot_chat/robot_main/Core/mylibs/board" -I"D:/ENSEA/Coursenensea/annee3/ESE/Systemes_a_microcontroleurs/TP/Projet_robot_chat/robot_main/Core/mylibs/bumper" -I"D:/ENSEA/Coursenensea/annee3/ESE/Systemes_a_microcontroleurs/TP/Projet_robot_chat/robot_main/Core/mylibs/led" -I"D:/ENSEA/Coursenensea/annee3/ESE/Systemes_a_microcontroleurs/TP/Projet_robot_chat/robot_main/Core/mylibs/motor" -I"D:/ENSEA/Coursenensea/annee3/ESE/Systemes_a_microcontroleurs/TP/Projet_robot_chat/robot_main/Core/mylibs/shell" -I"D:/ENSEA/Coursenensea/annee3/ESE/Systemes_a_microcontroleurs/TP/Projet_robot_chat/robot_main/Core/mylibs/ydlidar" -I"D:/ENSEA/Coursenensea/annee3/ESE/Systemes_a_microcontroleurs/TP/Projet_robot_chat/robot_main/Core/mylibs" -I"D:/ENSEA/Coursenensea/annee3/ESE/Systemes_a_microcontroleurs/TP/Projet_robot_chat/robot_main/Core/mylibs/control" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-mylibs-2f-ydlidar

clean-Core-2f-mylibs-2f-ydlidar:
	-$(RM) ./Core/mylibs/ydlidar/Ydlidar.cyclo ./Core/mylibs/ydlidar/Ydlidar.d ./Core/mylibs/ydlidar/Ydlidar.o ./Core/mylibs/ydlidar/Ydlidar.su

.PHONY: clean-Core-2f-mylibs-2f-ydlidar

