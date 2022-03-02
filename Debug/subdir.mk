################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../input_reader.cpp \
../main.cpp \
../stat_reader.cpp \
../transport_catalogue.cpp 

OBJS += \
./input_reader.o \
./main.o \
./stat_reader.o \
./transport_catalogue.o 

CPP_DEPS += \
./input_reader.d \
./main.d \
./stat_reader.d \
./transport_catalogue.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++17 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


