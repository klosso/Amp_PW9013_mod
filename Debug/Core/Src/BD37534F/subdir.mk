################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/Src/BD37534F/bd37534f.cpp 

OBJS += \
./Core/Src/BD37534F/bd37534f.o 

CPP_DEPS += \
./Core/Src/BD37534F/bd37534f.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/BD37534F/%.o Core/Src/BD37534F/%.su: ../Core/Src/BD37534F/%.cpp Core/Src/BD37534F/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m3 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -DSTM32F1xx=1 -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fexceptions -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-BD37534F

clean-Core-2f-Src-2f-BD37534F:
	-$(RM) ./Core/Src/BD37534F/bd37534f.d ./Core/Src/BD37534F/bd37534f.o ./Core/Src/BD37534F/bd37534f.su

.PHONY: clean-Core-2f-Src-2f-BD37534F

