################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Main_Funcs.c \
../Core/Src/enocoder.c \
../Core/Src/main.c \
../Core/Src/packing.pb.c \
../Core/Src/pb_common.c \
../Core/Src/pb_decode.c \
../Core/Src/pb_encode.c \
../Core/Src/stm32g4xx_hal_msp.c \
../Core/Src/stm32g4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32g4xx.c \
../Core/Src/uart.pb.c 

OBJS += \
./Core/Src/Main_Funcs.o \
./Core/Src/enocoder.o \
./Core/Src/main.o \
./Core/Src/packing.pb.o \
./Core/Src/pb_common.o \
./Core/Src/pb_decode.o \
./Core/Src/pb_encode.o \
./Core/Src/stm32g4xx_hal_msp.o \
./Core/Src/stm32g4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32g4xx.o \
./Core/Src/uart.pb.o 

C_DEPS += \
./Core/Src/Main_Funcs.d \
./Core/Src/enocoder.d \
./Core/Src/main.d \
./Core/Src/packing.pb.d \
./Core/Src/pb_common.d \
./Core/Src/pb_decode.d \
./Core/Src/pb_encode.d \
./Core/Src/stm32g4xx_hal_msp.d \
./Core/Src/stm32g4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32g4xx.d \
./Core/Src/uart.pb.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g -DDEBUG -DUSE_HAL_DRIVER -DSTM32G491xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -Ofast -ffunction-sections -fdata-sections -fno-strict-aliasing -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/Main_Funcs.cyclo ./Core/Src/Main_Funcs.d ./Core/Src/Main_Funcs.o ./Core/Src/Main_Funcs.su ./Core/Src/enocoder.cyclo ./Core/Src/enocoder.d ./Core/Src/enocoder.o ./Core/Src/enocoder.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/packing.pb.cyclo ./Core/Src/packing.pb.d ./Core/Src/packing.pb.o ./Core/Src/packing.pb.su ./Core/Src/pb_common.cyclo ./Core/Src/pb_common.d ./Core/Src/pb_common.o ./Core/Src/pb_common.su ./Core/Src/pb_decode.cyclo ./Core/Src/pb_decode.d ./Core/Src/pb_decode.o ./Core/Src/pb_decode.su ./Core/Src/pb_encode.cyclo ./Core/Src/pb_encode.d ./Core/Src/pb_encode.o ./Core/Src/pb_encode.su ./Core/Src/stm32g4xx_hal_msp.cyclo ./Core/Src/stm32g4xx_hal_msp.d ./Core/Src/stm32g4xx_hal_msp.o ./Core/Src/stm32g4xx_hal_msp.su ./Core/Src/stm32g4xx_it.cyclo ./Core/Src/stm32g4xx_it.d ./Core/Src/stm32g4xx_it.o ./Core/Src/stm32g4xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32g4xx.cyclo ./Core/Src/system_stm32g4xx.d ./Core/Src/system_stm32g4xx.o ./Core/Src/system_stm32g4xx.su ./Core/Src/uart.pb.cyclo ./Core/Src/uart.pb.d ./Core/Src/uart.pb.o ./Core/Src/uart.pb.su

.PHONY: clean-Core-2f-Src

