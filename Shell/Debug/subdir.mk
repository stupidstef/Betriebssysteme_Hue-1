################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../interpretiere.o \
../kommandos.o \
../listen.o \
../parser.o \
../prozess.o \
../scanner.o \
../shell.o \
../utils.o \
../wortspeicher.o 

C_SRCS += \
../interpretiere.c \
../kommandos.c \
../listen.c \
../parser.c \
../prozess.c \
../scanner.c \
../shell.c \
../utils.c \
../wortspeicher.c 

OBJS += \
./interpretiere.o \
./kommandos.o \
./listen.o \
./parser.o \
./prozess.o \
./scanner.o \
./shell.o \
./utils.o \
./wortspeicher.o 

C_DEPS += \
./interpretiere.d \
./kommandos.d \
./listen.d \
./parser.d \
./prozess.d \
./scanner.d \
./shell.d \
./utils.d \
./wortspeicher.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


