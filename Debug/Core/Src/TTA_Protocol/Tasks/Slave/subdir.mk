################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/TTA_Protocol/Tasks/Slave/task_slave_cmdexec.c \
../Core/Src/TTA_Protocol/Tasks/Slave/task_slave_gpio.c \
../Core/Src/TTA_Protocol/Tasks/Slave/task_slave_rx.c \
../Core/Src/TTA_Protocol/Tasks/Slave/task_slave_tx.c 

OBJS += \
./Core/Src/TTA_Protocol/Tasks/Slave/task_slave_cmdexec.o \
./Core/Src/TTA_Protocol/Tasks/Slave/task_slave_gpio.o \
./Core/Src/TTA_Protocol/Tasks/Slave/task_slave_rx.o \
./Core/Src/TTA_Protocol/Tasks/Slave/task_slave_tx.o 

C_DEPS += \
./Core/Src/TTA_Protocol/Tasks/Slave/task_slave_cmdexec.d \
./Core/Src/TTA_Protocol/Tasks/Slave/task_slave_gpio.d \
./Core/Src/TTA_Protocol/Tasks/Slave/task_slave_rx.d \
./Core/Src/TTA_Protocol/Tasks/Slave/task_slave_tx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/TTA_Protocol/Tasks/Slave/%.o Core/Src/TTA_Protocol/Tasks/Slave/%.su Core/Src/TTA_Protocol/Tasks/Slave/%.cyclo: ../Core/Src/TTA_Protocol/Tasks/Slave/%.c Core/Src/TTA_Protocol/Tasks/Slave/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L412xx -c -I"/Users/sureshlama/Bootloader_L4Q5VGT6/RTOS_STM32L412RB/Drivers/CMSIS/DSP/Include" -I"/Users/sureshlama/Bootloader_L4Q5VGT6/RTOS_STM32L412RB/Drivers/CMSIS/RTOS2/Include" -I"/Users/sureshlama/Bootloader_L4Q5VGT6/RTOS_STM32L412RB/Drivers/CMSIS/NN/Include" -I"/Users/sureshlama/Bootloader_L4Q5VGT6/RTOS_STM32L412RB/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F" -I"/Users/sureshlama/Bootloader_L4Q5VGT6/RTOS_STM32L412RB/Middlewares/Third_Party/FreeRTOS/Source/include" -I"/Users/sureshlama/Bootloader_L4Q5VGT6/RTOS_STM32L412RB/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2" -I../Core/Inc -I"/Users/sureshlama/Bootloader_L4Q5VGT6/RTOS_STM32L412RB/Middlewares/Third_Party/FreeRTOS/Source/include" -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-TTA_Protocol-2f-Tasks-2f-Slave

clean-Core-2f-Src-2f-TTA_Protocol-2f-Tasks-2f-Slave:
	-$(RM) ./Core/Src/TTA_Protocol/Tasks/Slave/task_slave_cmdexec.cyclo ./Core/Src/TTA_Protocol/Tasks/Slave/task_slave_cmdexec.d ./Core/Src/TTA_Protocol/Tasks/Slave/task_slave_cmdexec.o ./Core/Src/TTA_Protocol/Tasks/Slave/task_slave_cmdexec.su ./Core/Src/TTA_Protocol/Tasks/Slave/task_slave_gpio.cyclo ./Core/Src/TTA_Protocol/Tasks/Slave/task_slave_gpio.d ./Core/Src/TTA_Protocol/Tasks/Slave/task_slave_gpio.o ./Core/Src/TTA_Protocol/Tasks/Slave/task_slave_gpio.su ./Core/Src/TTA_Protocol/Tasks/Slave/task_slave_rx.cyclo ./Core/Src/TTA_Protocol/Tasks/Slave/task_slave_rx.d ./Core/Src/TTA_Protocol/Tasks/Slave/task_slave_rx.o ./Core/Src/TTA_Protocol/Tasks/Slave/task_slave_rx.su ./Core/Src/TTA_Protocol/Tasks/Slave/task_slave_tx.cyclo ./Core/Src/TTA_Protocol/Tasks/Slave/task_slave_tx.d ./Core/Src/TTA_Protocol/Tasks/Slave/task_slave_tx.o ./Core/Src/TTA_Protocol/Tasks/Slave/task_slave_tx.su

.PHONY: clean-Core-2f-Src-2f-TTA_Protocol-2f-Tasks-2f-Slave

