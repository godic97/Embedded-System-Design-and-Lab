################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lcd.c \
../term.c \
../touch.c 

OBJS += \
./lcd.o \
./term.o \
./touch.o 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM C Compiler 5'
	armcc -I"C:\Users\Team07\Desktop\term\term\Libraries\CMSIS\CoreSupport" -I"C:\Users\Team07\Desktop\term\term" -I"C:\Users\Team07\Desktop\term\term\Libraries\CMSIS\DeviceSupport" -I"C:\Users\Team07\Desktop\term\term\Libraries\CMSIS\DeviceSupport\Startup" -I"C:\Users\Team07\Desktop\term\term\Libraries\STM32F10x_StdPeriph_Driver_v3.5\inc" -I"C:\Users\Team07\Desktop\term\term\Libraries\STM32F10x_StdPeriph_Driver_v3.5\src" -O3 --cpu=cortex-m3 -g -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

term.o: ../term.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM C Compiler 5'
	armcc -I"C:\Users\Team07\Desktop\term\term\Libraries\CMSIS\CoreSupport" -I"C:\Users\Team07\Desktop\term\term" -I"C:\Users\Team07\Desktop\term\term\Libraries\CMSIS\DeviceSupport" -I"C:\Users\Team07\Desktop\term\term\Libraries\CMSIS\DeviceSupport\Startup" -I"C:\Users\Team07\Desktop\term\term\Libraries\STM32F10x_StdPeriph_Driver_v3.5\inc" -I"C:\Users\Team07\Desktop\term\term\Libraries\STM32F10x_StdPeriph_Driver_v3.5\src" -O2 --cpu=cortex-m3 -g -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


