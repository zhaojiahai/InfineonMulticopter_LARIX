################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Dave/Generated/src/USBCORE001/usbcore001.c \
../Dave/Generated/src/USBCORE001/usbcore001_Conf.c \
../Dave/Generated/src/USBCORE001/usbcore001_device.c \
../Dave/Generated/src/USBCORE001/usbcore001_host.c 

OBJS += \
./Dave/Generated/src/USBCORE001/usbcore001.o \
./Dave/Generated/src/USBCORE001/usbcore001_Conf.o \
./Dave/Generated/src/USBCORE001/usbcore001_device.o \
./Dave/Generated/src/USBCORE001/usbcore001_host.o 

C_DEPS += \
./Dave/Generated/src/USBCORE001/usbcore001.d \
./Dave/Generated/src/USBCORE001/usbcore001_Conf.d \
./Dave/Generated/src/USBCORE001/usbcore001_device.d \
./Dave/Generated/src/USBCORE001/usbcore001_host.d 


# Each subdirectory must supply rules for building sources it contributes
Dave/Generated/src/USBCORE001/%.o: ../Dave/Generated/src/USBCORE001/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM-GCC C Compiler'
	"C:\DAVE-3.1.10\ARM-GCC/bin/arm-none-eabi-gcc" -DDAVE_CE -DARM_MATH_CM4 -D_FPU_PRESENT -DUC_ID=4503003 -I"C:\DAVE-3.1.10\eclipse\/../CMSIS/Include" -I"C:\DAVE-3.1.10\eclipse\/../CMSIS/Infineon/Include" -I"C:\DAVE-3.1.10\ARM-GCC/arm-none-eabi/include" -I"C:\DAVE-3.1.10\eclipse\/../emWin/Start/GUI/inc" -I"C:\DAVE-3.1.10\eclipse\/../CMSIS/Infineon/XMC4500_series/Include" -I"C:\DAVE3_workspace\ws3.1.10\CORE_Larix_V1.0\Dave\Generated\inc\DAVESupport" -I"C:\DAVE3_workspace\ws3.1.10\CORE_Larix_V1.0\Dave\Generated\inc\LIBS" -I"C:\DAVE3_workspace\ws3.1.10\CORE_Larix_V1.0\Dave\Generated\inc\MOTORLIBS" -I"C:\DAVE3_workspace\ws3.1.10\CORE_Larix_V1.0\Dave\Generated\src\USBCDC001\Drivers\USB\Core" -I"C:\DAVE3_workspace\ws3.1.10\CORE_Larix_V1.0\Dave\Generated\src\USBCDC001\Drivers\USB" -I"C:\DAVE3_workspace\ws3.1.10\CORE_Larix_V1.0\Dave\Generated\src\USBCDC001\Drivers\USB\Class" -Os -ffunction-sections -Wall -std=gnu99 -mfloat-abi=softfp -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $@" -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


