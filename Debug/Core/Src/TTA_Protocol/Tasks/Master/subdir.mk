################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/TTA_Protocol/Tasks/Master/task_master_button.c \
../Core/Src/TTA_Protocol/Tasks/Master/task_master_protocol.c \
../Core/Src/TTA_Protocol/Tasks/Master/task_master_rx.c \
../Core/Src/TTA_Protocol/Tasks/Master/task_master_tx.c 

OBJS += \
./Core/Src/TTA_Protocol/Tasks/Master/task_master_button.o \
./Core/Src/TTA_Protocol/Tasks/Master/task_master_protocol.o \
./Core/Src/TTA_Protocol/Tasks/Master/task_master_rx.o \
./Core/Src/TTA_Protocol/Tasks/Master/task_master_tx.o 

C_DEPS += \
./Core/Src/TTA_Protocol/Tasks/Master/task_master_button.d \
./Core/Src/TTA_Protocol/Tasks/Master/task_master_protocol.d \
./Core/Src/TTA_Protocol/Tasks/Master/task_master_rx.d \
./Core/Src/TTA_Protocol/Tasks/Master/task_master_tx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/TTA_Protocol/Tasks/Master/%.o Core/Src/TTA_Protocol/Tasks/Master/%.su Core/Src/TTA_Protocol/Tasks/Master/%.cyclo: ../Core/Src/TTA_Protocol/Tasks/Master/%.c Core/Src/TTA_Protocol/Tasks/Master/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L412xx -c -I"/Users/sureshlama/Bootloader_L4Q5VGT6/RTOS_STM32L412RB/Drivers/CMSIS/DSP/Include" -I"/Users/sureshlama/Bootloader_L4Q5VGT6/RTOS_STM32L412RB/Drivers/CMSIS/RTOS2/Include" -I"/Users/sureshlama/Bootloader_L4Q5VGT6/RTOS_STM32L412RB/Drivers/CMSIS/NN/Include" -I"/Users/sureshlama/Bootloader_L4Q5VGT6/RTOS_STM32L412RB/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F" -I"/Users/sureshlama/Bootloader_L4Q5VGT6/RTOS_STM32L412RB/Middlewares/Third_Party/FreeRTOS/Source/include" -I"/Users/sureshlama/Bootloader_L4Q5VGT6/RTOS_STM32L412RB/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2" -I../Core/Inc -I"/Users/sureshlama/Bootloader_L4Q5VGT6/RTOS_STM32L412RB/Middlewares/Third_Party/FreeRTOS/Source/include" -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-TTA_Protocol-2f-Tasks-2f-Master

clean-Core-2f-Src-2f-TTA_Protocol-2f-Tasks-2f-Master:
	-$(RM) ./Core/Src/TTA_Protocol/Tasks/Master/task_master_button.cyclo ./Core/Src/TTA_Protocol/Tasks/Master/task_master_button.d ./Core/Src/TTA_Protocol/Tasks/Master/task_master_button.o ./Core/Src/TTA_Protocol/Tasks/Master/task_master_button.su ./Core/Src/TTA_Protocol/Tasks/Master/task_master_protocol.cyclo ./Core/Src/TTA_Protocol/Tasks/Master/task_master_protocol.d ./Core/Src/TTA_Protocol/Tasks/Master/task_master_protocol.o ./Core/Src/TTA_Protocol/Tasks/Master/task_master_protocol.su ./Core/Src/TTA_Protocol/Tasks/Master/task_master_rx.cyclo ./Core/Src/TTA_Protocol/Tasks/Master/task_master_rx.d ./Core/Src/TTA_Protocol/Tasks/Master/task_master_rx.o ./Core/Src/TTA_Protocol/Tasks/Master/task_master_rx.su ./Core/Src/TTA_Protocol/Tasks/Master/task_master_tx.cyclo ./Core/Src/TTA_Protocol/Tasks/Master/task_master_tx.d ./Core/Src/TTA_Protocol/Tasks/Master/task_master_tx.o ./Core/Src/TTA_Protocol/Tasks/Master/task_master_tx.su

.PHONY: clean-Core-2f-Src-2f-TTA_Protocol-2f-Tasks-2f-Master

