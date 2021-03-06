################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Dc-motor.c \
../buzzer.c \
../external_eeprom.c \
../gpio.c \
../i2c.c \
../mc2.c \
../timer.c \
../uart.c 

OBJS += \
./Dc-motor.o \
./buzzer.o \
./external_eeprom.o \
./gpio.o \
./i2c.o \
./mc2.o \
./timer.o \
./uart.o 

C_DEPS += \
./Dc-motor.d \
./buzzer.d \
./external_eeprom.d \
./gpio.d \
./i2c.d \
./mc2.d \
./timer.d \
./uart.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=1000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


