################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/freertos/port_freertos_hook.c 

OBJS += \
./src/freertos/port_freertos_hook.o 

C_DEPS += \
./src/freertos/port_freertos_hook.d 


# Each subdirectory must supply rules for building sources it contributes
src/freertos/%.o: ../src/freertos/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v8 gcc compiler'
	aarch64-none-elf-gcc -Wall -O0 -g3 -c -fmessage-length=0 -MT"$@" -ID:/code/2Wproject/D3project/top/export/top/sw/top/freertos10_xilinx_psu_cortexa53_0/bspinclude/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


