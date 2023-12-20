################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/mylibs/motor/PIDControl.c \
../Core/mylibs/motor/dcMotor.c \
../Core/mylibs/motor/getEncoder.c \
../Core/mylibs/motor/setSpeed.c 

OBJS += \
./Core/mylibs/motor/PIDControl.o \
./Core/mylibs/motor/dcMotor.o \
./Core/mylibs/motor/getEncoder.o \
./Core/mylibs/motor/setSpeed.o 

C_DEPS += \
./Core/mylibs/motor/PIDControl.d \
./Core/mylibs/motor/dcMotor.d \
./Core/mylibs/motor/getEncoder.d \
./Core/mylibs/motor/setSpeed.d 


# Each subdirectory must supply rules for building sources it contributes
Core/mylibs/motor/%.o Core/mylibs/motor/%.su Core/mylibs/motor/%.cyclo: ../Core/mylibs/motor/%.c Core/mylibs/motor/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G070xx -c -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0 -I"D:/ENSEA/Coursenensea/annee3/ESE/Systemes_a_microcontroleurs/TP/Projet_robot_chat/robot_main/Core/mylibs/board" -I"D:/ENSEA/Coursenensea/annee3/ESE/Systemes_a_microcontroleurs/TP/Projet_robot_chat/robot_main/Core/mylibs/bumper" -I"D:/ENSEA/Coursenensea/annee3/ESE/Systemes_a_microcontroleurs/TP/Projet_robot_chat/robot_main/Core/mylibs/led" -I"D:/ENSEA/Coursenensea/annee3/ESE/Systemes_a_microcontroleurs/TP/Projet_robot_chat/robot_main/Core/mylibs/motor" -I"D:/ENSEA/Coursenensea/annee3/ESE/Systemes_a_microcontroleurs/TP/Projet_robot_chat/robot_main/Core/mylibs/shell" -I"D:/ENSEA/Coursenensea/annee3/ESE/Systemes_a_microcontroleurs/TP/Projet_robot_chat/robot_main/Core/mylibs/ydlidar" -I"D:/ENSEA/Coursenensea/annee3/ESE/Systemes_a_microcontroleurs/TP/Projet_robot_chat/robot_main/Core/mylibs" -I"D:/ENSEA/Coursenensea/annee3/ESE/Systemes_a_microcontroleurs/TP/Projet_robot_chat/robot_main/Core/mylibs/control" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-mylibs-2f-motor

clean-Core-2f-mylibs-2f-motor:
	-$(RM) ./Core/mylibs/motor/PIDControl.cyclo ./Core/mylibs/motor/PIDControl.d ./Core/mylibs/motor/PIDControl.o ./Core/mylibs/motor/PIDControl.su ./Core/mylibs/motor/dcMotor.cyclo ./Core/mylibs/motor/dcMotor.d ./Core/mylibs/motor/dcMotor.o ./Core/mylibs/motor/dcMotor.su ./Core/mylibs/motor/getEncoder.cyclo ./Core/mylibs/motor/getEncoder.d ./Core/mylibs/motor/getEncoder.o ./Core/mylibs/motor/getEncoder.su ./Core/mylibs/motor/setSpeed.cyclo ./Core/mylibs/motor/setSpeed.d ./Core/mylibs/motor/setSpeed.o ./Core/mylibs/motor/setSpeed.su

.PHONY: clean-Core-2f-mylibs-2f-motor

