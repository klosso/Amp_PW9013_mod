################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/Src/CFPushButton/CFPushButton.cpp 

OBJS += \
./Core/Src/CFPushButton/CFPushButton.o 

CPP_DEPS += \
./Core/Src/CFPushButton/CFPushButton.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/CFPushButton/%.o Core/Src/CFPushButton/%.su: ../Core/Src/CFPushButton/%.cpp Core/Src/CFPushButton/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m3 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -DSTM32F1xx=1 -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fexceptions -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-CFPushButton

clean-Core-2f-Src-2f-CFPushButton:
	-$(RM) ./Core/Src/CFPushButton/CFPushButton.d ./Core/Src/CFPushButton/CFPushButton.o ./Core/Src/CFPushButton/CFPushButton.su

.PHONY: clean-Core-2f-Src-2f-CFPushButton

