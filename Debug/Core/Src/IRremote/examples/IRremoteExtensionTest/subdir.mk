################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/Src/IRremote/examples/IRremoteExtensionTest/IRremoteExtensionClass.cpp 

OBJS += \
./Core/Src/IRremote/examples/IRremoteExtensionTest/IRremoteExtensionClass.o 

CPP_DEPS += \
./Core/Src/IRremote/examples/IRremoteExtensionTest/IRremoteExtensionClass.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/IRremote/examples/IRremoteExtensionTest/%.o: ../Core/Src/IRremote/examples/IRremoteExtensionTest/%.cpp Core/Src/IRremote/examples/IRremoteExtensionTest/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m3 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-IRremote-2f-examples-2f-IRremoteExtensionTest

clean-Core-2f-Src-2f-IRremote-2f-examples-2f-IRremoteExtensionTest:
	-$(RM) ./Core/Src/IRremote/examples/IRremoteExtensionTest/IRremoteExtensionClass.d ./Core/Src/IRremote/examples/IRremoteExtensionTest/IRremoteExtensionClass.o

.PHONY: clean-Core-2f-Src-2f-IRremote-2f-examples-2f-IRremoteExtensionTest

