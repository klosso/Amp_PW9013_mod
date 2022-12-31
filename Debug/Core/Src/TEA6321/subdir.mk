################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/Src/TEA6321/tea6321.cpp 

OBJS += \
./Core/Src/TEA6321/tea6321.o 

CPP_DEPS += \
./Core/Src/TEA6321/tea6321.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/TEA6321/%.o Core/Src/TEA6321/%.su: ../Core/Src/TEA6321/%.cpp Core/Src/TEA6321/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m3 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -DSTM32F1xx=1 -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fexceptions -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-TEA6321

clean-Core-2f-Src-2f-TEA6321:
	-$(RM) ./Core/Src/TEA6321/tea6321.d ./Core/Src/TEA6321/tea6321.o ./Core/Src/TEA6321/tea6321.su

.PHONY: clean-Core-2f-Src-2f-TEA6321

