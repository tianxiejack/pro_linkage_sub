################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/CcCamCalibra.cpp \
../src/Displayer.cpp \
../src/MultiChVideo.cpp \
../src/VideoProcess.cpp \
../src/WorkThread.cpp \
../src/autoManualFindRelation.cpp \
../src/cuda_mem.cpp \
../src/link.cpp \
../src/main.cpp \
../src/menu.cpp \
../src/process51.cpp \
../src/sceneProcess.cpp \
../src/state.cpp \
../src/stateManager.cpp \
../src/state_linkAuto.cpp \
../src/state_linkBall.cpp \
../src/state_linkCalib.cpp \
../src/state_linkManual.cpp \
../src/v4l2camera.cpp 

CU_SRCS += \
../src/cuda.cu \
../src/cuda_convert.cu 

CU_DEPS += \
./src/cuda.d \
./src/cuda_convert.d 

OBJS += \
./src/CcCamCalibra.o \
./src/Displayer.o \
./src/MultiChVideo.o \
./src/VideoProcess.o \
./src/WorkThread.o \
./src/autoManualFindRelation.o \
./src/cuda.o \
./src/cuda_convert.o \
./src/cuda_mem.o \
./src/link.o \
./src/main.o \
./src/menu.o \
./src/process51.o \
./src/sceneProcess.o \
./src/state.o \
./src/stateManager.o \
./src/state_linkAuto.o \
./src/state_linkBall.o \
./src/state_linkCalib.o \
./src/state_linkManual.o \
./src/v4l2camera.o 

CPP_DEPS += \
./src/CcCamCalibra.d \
./src/Displayer.d \
./src/MultiChVideo.d \
./src/VideoProcess.d \
./src/WorkThread.d \
./src/autoManualFindRelation.d \
./src/cuda_mem.d \
./src/link.d \
./src/main.d \
./src/menu.d \
./src/process51.d \
./src/sceneProcess.d \
./src/state.d \
./src/stateManager.d \
./src/state_linkAuto.d \
./src/state_linkBall.d \
./src/state_linkCalib.d \
./src/state_linkManual.d \
./src/v4l2camera.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	/usr/local/cuda-8.0/bin/nvcc -D__IPC__=1 -D__TRACK__=1 -D__MOVE_DETECT__=1 -I/usr/lib/aarch64-linux-gnu/include -I../src/DxTimer -I/usr/include/freetype2 -I/usr/include/opencv -I/usr/include/opencv2 -I/usr/include/GL -I../include -I../include/osa -I../src/GST -G -g -O0 -Xcompiler -fopenmp -std=c++11 -ccbin aarch64-linux-gnu-g++ -gencode arch=compute_20,code=sm_20 -gencode arch=compute_53,code=sm_53 -m64 -odir "src" -M -o "$(@:%.o=%.d)" "$<"
	/usr/local/cuda-8.0/bin/nvcc -D__IPC__=1 -D__TRACK__=1 -D__MOVE_DETECT__=1 -I/usr/lib/aarch64-linux-gnu/include -I../src/DxTimer -I/usr/include/freetype2 -I/usr/include/opencv -I/usr/include/opencv2 -I/usr/include/GL -I../include -I../include/osa -I../src/GST -G -g -O0 -Xcompiler -fopenmp -std=c++11 --compile -m64 -ccbin aarch64-linux-gnu-g++  -x c++ -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.cu
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	/usr/local/cuda-8.0/bin/nvcc -D__IPC__=1 -D__TRACK__=1 -D__MOVE_DETECT__=1 -I/usr/lib/aarch64-linux-gnu/include -I../src/DxTimer -I/usr/include/freetype2 -I/usr/include/opencv -I/usr/include/opencv2 -I/usr/include/GL -I../include -I../include/osa -I../src/GST -G -g -O0 -Xcompiler -fopenmp -std=c++11 -ccbin aarch64-linux-gnu-g++ -gencode arch=compute_20,code=sm_20 -gencode arch=compute_53,code=sm_53 -m64 -odir "src" -M -o "$(@:%.o=%.d)" "$<"
	/usr/local/cuda-8.0/bin/nvcc -D__IPC__=1 -D__TRACK__=1 -D__MOVE_DETECT__=1 -I/usr/lib/aarch64-linux-gnu/include -I../src/DxTimer -I/usr/include/freetype2 -I/usr/include/opencv -I/usr/include/opencv2 -I/usr/include/GL -I../include -I../include/osa -I../src/GST -G -g -O0 -Xcompiler -fopenmp -std=c++11 --compile --relocatable-device-code=false -gencode arch=compute_20,code=compute_20 -gencode arch=compute_53,code=compute_53 -gencode arch=compute_20,code=sm_20 -gencode arch=compute_53,code=sm_53 -m64 -ccbin aarch64-linux-gnu-g++  -x cu -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


