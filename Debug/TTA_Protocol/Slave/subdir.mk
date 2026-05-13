################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../TTA_Protocol/Slave/slave_fsm.c \
../TTA_Protocol/Slave/slave_gpio_logic.c \
../TTA_Protocol/Slave/slave_scheduler.c 

OBJS += \
./TTA_Protocol/Slave/slave_fsm.o \
./TTA_Protocol/Slave/slave_gpio_logic.o \
./TTA_Protocol/Slave/slave_scheduler.o 

C_DEPS += \
./TTA_Protocol/Slave/slave_fsm.d \
./TTA_Protocol/Slave/slave_gpio_logic.d \
./TTA_Protocol/Slave/slave_scheduler.d 


# Each subdirectory must supply rules for building sources it contributes
TTA_Protocol/Slave/%.o TTA_Protocol/Slave/%.su TTA_Protocol/Slave/%.cyclo: ../TTA_Protocol/Slave/%.c TTA_Protocol/Slave/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L412xx -c -I"/Users/sureshlama/Bootloader_L4Q5VGT6/RTOS_STM32L412RB/Drivers/CMSIS/DSP/Include" -I"/Users/sureshlama/Bootloader_L4Q5VGT6/RTOS_STM32L412RB/TTA_Protocol/Common" -I"/Users/sureshlama/Bootloader_L4Q5VGT6/RTOS_STM32L412RB/TTA_Protocol/Master" -I"/Users/sureshlama/Bootloader_L4Q5VGT6/RTOS_STM32L412RB/TTA_Protocol/Tasks" -I"/Users/sureshlama/Bootloader_L4Q5VGT6/RTOS_STM32L412RB/TTA_Protocol/Slave" -I"/Users/sureshlama/Bootloader_L4Q5VGT6/RTOS_STM32L412RB/TTA_Protocol" -I"/Users/sureshlama/Bootloader_L4Q5VGT6/RTOS_STM32L412RB/TTA_Protocol" -I"/Users/sureshlama/Bootloader_L4Q5VGT6/RTOS_STM32L412RB/Drivers/CMSIS/RTOS2/Include" -I"/Users/sureshlama/Bootloader_L4Q5VGT6/RTOS_STM32L412RB/Drivers/CMSIS/NN/Include" -I"/Users/sureshlama/Bootloader_L4Q5VGT6/RTOS_STM32L412RB/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F" -I"/Users/sureshlama/Bootloader_L4Q5VGT6/RTOS_STM32L412RB/Middlewares/Third_Party/FreeRTOS/Source/include" -I"/Users/sureshlama/Bootloader_L4Q5VGT6/RTOS_STM32L412RB/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2" -I../Core/Inc -I"/Users/sureshlama/Bootloader_L4Q5VGT6/RTOS_STM32L412RB/Middlewares/Third_Party/FreeRTOS/Source/include" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I"/Users/sureshlama/Bootloader_L4Q5VGT6/RTOS_STM32L412RB/Drivers/CMSIS/Device/ST/STM32L4xx/Include" -I../Drivers/STM32L4xx_HAL_Driver/Inc -I"/Users/sureshlama/Bootloader_L4Q5VGT6/RTOS_STM32L412RB/Drivers/STM32L4xx_HAL_Driver/Inc" -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I"/Users/sureshlama/Bootloader_L4Q5VGT6/RTOS_STM32L412RB/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy" -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I"/Users/sureshlama/Bootloader_L4Q5VGT6/RTOS_STM32L412RB/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2" -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"/Users/sureshlama/Bootloader_L4Q5VGT6/RTOS_STM32L412RB/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F" -I../Drivers/CMSIS/Include -I"/Users/sureshlama/Bootloader_L4Q5VGT6/RTOS_STM32L412RB/Drivers/CMSIS/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-TTA_Protocol-2f-Slave

clean-TTA_Protocol-2f-Slave:
	-$(RM) ./TTA_Protocol/Slave/slave_fsm.cyclo ./TTA_Protocol/Slave/slave_fsm.d ./TTA_Protocol/Slave/slave_fsm.o ./TTA_Protocol/Slave/slave_fsm.su ./TTA_Protocol/Slave/slave_gpio_logic.cyclo ./TTA_Protocol/Slave/slave_gpio_logic.d ./TTA_Protocol/Slave/slave_gpio_logic.o ./TTA_Protocol/Slave/slave_gpio_logic.su ./TTA_Protocol/Slave/slave_scheduler.cyclo ./TTA_Protocol/Slave/slave_scheduler.d ./TTA_Protocol/Slave/slave_scheduler.o ./TTA_Protocol/Slave/slave_scheduler.su

.PHONY: clean-TTA_Protocol-2f-Slave

