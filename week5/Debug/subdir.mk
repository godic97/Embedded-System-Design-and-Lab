################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../week5.c 

OBJS += \
./week5.o 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM C++ Compiler 5'
	armcc -I"C:\Users\Team03\Documents\DS-5 Workspace\week5_2\Libraries\CMSIS\CoreSupport" -I"C:\Users\Team03\Documents\DS-5 Workspace\week5_2\Libraries\CMSIS\DeviceSupport" -I"C:\Users\Team03\Documents\DS-5 Workspace\week5_2\Libraries\CMSIS\DeviceSupport\Startup" -I"C:\Users\Team03\Documents\DS-5 Workspace\week5_2\Libraries\STM32F10x_StdPeriph_Driver_v3.5\inc" -I"C:\Users\Team03\Documents\DS-5 Workspace\week5_2\Libraries\STM32F10x_StdPeriph_Driver_v3.5\src" --cpp -O0 --cpu=cortex-m3 -g -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


