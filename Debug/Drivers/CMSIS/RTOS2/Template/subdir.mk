################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CMSIS/RTOS2/Template/cmsis_os1.c 

OBJS += \
./Drivers/CMSIS/RTOS2/Template/cmsis_os1.o 

C_DEPS += \
./Drivers/CMSIS/RTOS2/Template/cmsis_os1.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS/RTOS2/Template/%.o Drivers/CMSIS/RTOS2/Template/%.su Drivers/CMSIS/RTOS2/Template/%.cyclo: ../Drivers/CMSIS/RTOS2/Template/%.c Drivers/CMSIS/RTOS2/Template/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L412xx -c -I"/Users/sureshlama/Bootloader_L4Q5VGT6/RTOS_STM32L412RB/Drivers/CMSIS/DSP/Include" -I"/Users/sureshlama/Bootloader_L4Q5VGT6/RTOS_STM32L412RB/TTA_Protocol/Common" -I"/Users/sureshlama/Bootloader_L4Q5VGT6/RTOS_STM32L412RB/TTA_Protocol/Master" -I"/Users/sureshlama/Bootloader_L4Q5VGT6/RTOS_STM32L412RB/TTA_Protocol/Tasks" -I"/Users/sureshlama/Bootloader_L4Q5VGT6/RTOS_STM32L412RB/TTA_Protocol/Slave" -I"/Users/sureshlama/Bootloader_L4Q5VGT6/RTOS_STM32L412RB/TTA_Protocol" -I"/Users/sureshlama/Bootloader_L4Q5VGT6/RTOS_STM32L412RB/TTA_Protocol" -I"/Users/sureshlama/Bootloader_L4Q5VGT6/RTOS_STM32L412RB/Drivers/CMSIS/RTOS2/Include" -I"/Users/sureshlama/Bootloader_L4Q5VGT6/RTOS_STM32L412RB/Drivers/CMSIS/NN/Include" -I"/Users/sureshlama/Bootloader_L4Q5VGT6/RTOS_STM32L412RB/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F" -I"/Users/sureshlama/Bootloader_L4Q5VGT6/RTOS_STM32L412RB/Middlewares/Third_Party/FreeRTOS/Source/include" -I"/Users/sureshlama/Bootloader_L4Q5VGT6/RTOS_STM32L412RB/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2" -I../Core/Inc -I"/Users/sureshlama/Bootloader_L4Q5VGT6/RTOS_STM32L412RB/Middlewares/Third_Party/FreeRTOS/Source/include" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I"/Users/sureshlama/Bootloader_L4Q5VGT6/RTOS_STM32L412RB/Drivers/CMSIS/Device/ST/STM32L4xx/Include" -I../Drivers/STM32L4xx_HAL_Driver/Inc -I"/Users/sureshlama/Bootloader_L4Q5VGT6/RTOS_STM32L412RB/Drivers/STM32L4xx_HAL_Driver/Inc" -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I"/Users/sureshlama/Bootloader_L4Q5VGT6/RTOS_STM32L412RB/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy" -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I"/Users/sureshlama/Bootloader_L4Q5VGT6/RTOS_STM32L412RB/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2" -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"/Users/sureshlama/Bootloader_L4Q5VGT6/RTOS_STM32L412RB/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F" -I../Drivers/CMSIS/Include -I"/Users/sureshlama/Bootloader_L4Q5VGT6/RTOS_STM32L412RB/Drivers/CMSIS/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-CMSIS-2f-RTOS2-2f-Template

clean-Drivers-2f-CMSIS-2f-RTOS2-2f-Template:
	-$(RM) ./Drivers/CMSIS/RTOS2/Template/cmsis_os1.cyclo ./Drivers/CMSIS/RTOS2/Template/cmsis_os1.d ./Drivers/CMSIS/RTOS2/Template/cmsis_os1.o ./Drivers/CMSIS/RTOS2/Template/cmsis_os1.su

.PHONY: clean-Drivers-2f-CMSIS-2f-RTOS2-2f-Template

