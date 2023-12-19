################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../mylibs/PIDControl.c \
../mylibs/dcMotor.c \
../mylibs/getEncoder.c \
../mylibs/setSpeed.c \
../mylibs/shell.c 

OBJS += \
./mylibs/PIDControl.o \
./mylibs/dcMotor.o \
./mylibs/getEncoder.o \
./mylibs/setSpeed.o \
./mylibs/shell.o 

C_DEPS += \
./mylibs/PIDControl.d \
./mylibs/dcMotor.d \
./mylibs/getEncoder.d \
./mylibs/setSpeed.d \
./mylibs/shell.d 


# Each subdirectory must supply rules for building sources it contributes
mylibs/%.o mylibs/%.su mylibs/%.cyclo: ../mylibs/%.c mylibs/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G070xx -c -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -I"E:/STM32CubeIDE/workspace/projet_systemelec/mylibs" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-mylibs

clean-mylibs:
	-$(RM) ./mylibs/PIDControl.cyclo ./mylibs/PIDControl.d ./mylibs/PIDControl.o ./mylibs/PIDControl.su ./mylibs/dcMotor.cyclo ./mylibs/dcMotor.d ./mylibs/dcMotor.o ./mylibs/dcMotor.su ./mylibs/getEncoder.cyclo ./mylibs/getEncoder.d ./mylibs/getEncoder.o ./mylibs/getEncoder.su ./mylibs/setSpeed.cyclo ./mylibs/setSpeed.d ./mylibs/setSpeed.o ./mylibs/setSpeed.su ./mylibs/shell.cyclo ./mylibs/shell.d ./mylibs/shell.o ./mylibs/shell.su

.PHONY: clean-mylibs

