################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/mylibs/bumper/bumper.c 

OBJS += \
./Core/mylibs/bumper/bumper.o 

C_DEPS += \
./Core/mylibs/bumper/bumper.d 


# Each subdirectory must supply rules for building sources it contributes
Core/mylibs/bumper/%.o Core/mylibs/bumper/%.su Core/mylibs/bumper/%.cyclo: ../Core/mylibs/bumper/%.c Core/mylibs/bumper/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G070xx -c -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0 -I"D:/ENSEA/Coursenensea/annee3/ESE/Systemes_a_microcontroleurs/TP/Projet_robot_chat/robot_main/Core/mylibs/board" -I"D:/ENSEA/Coursenensea/annee3/ESE/Systemes_a_microcontroleurs/TP/Projet_robot_chat/robot_main/Core/mylibs/bumper" -I"D:/ENSEA/Coursenensea/annee3/ESE/Systemes_a_microcontroleurs/TP/Projet_robot_chat/robot_main/Core/mylibs/led" -I"D:/ENSEA/Coursenensea/annee3/ESE/Systemes_a_microcontroleurs/TP/Projet_robot_chat/robot_main/Core/mylibs/motor" -I"D:/ENSEA/Coursenensea/annee3/ESE/Systemes_a_microcontroleurs/TP/Projet_robot_chat/robot_main/Core/mylibs/shell" -I"D:/ENSEA/Coursenensea/annee3/ESE/Systemes_a_microcontroleurs/TP/Projet_robot_chat/robot_main/Core/mylibs/ydlidar" -I"D:/ENSEA/Coursenensea/annee3/ESE/Systemes_a_microcontroleurs/TP/Projet_robot_chat/robot_main/Core/mylibs" -I"D:/ENSEA/Coursenensea/annee3/ESE/Systemes_a_microcontroleurs/TP/Projet_robot_chat/robot_main/Core/mylibs/control" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-mylibs-2f-bumper

clean-Core-2f-mylibs-2f-bumper:
	-$(RM) ./Core/mylibs/bumper/bumper.cyclo ./Core/mylibs/bumper/bumper.d ./Core/mylibs/bumper/bumper.o ./Core/mylibs/bumper/bumper.su

.PHONY: clean-Core-2f-mylibs-2f-bumper

