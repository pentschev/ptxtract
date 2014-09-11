CC			:= gcc
NVCC		:= nvcc

CUDA_PATH	?= /usr/local/cuda

ifeq ($(shell uname -m), x86_64)
  LIBARCH=lib64
else
  LIBARCH=lib
endif

BIN_DIR		:= bin
KERNEL_DIR	:= $(BIN_DIR)/kernel
PTX_DIR		:= ptx
SRC_DIR		:= src
SRC_CUDA	:= $(SRC_DIR)/cuda
SRC_OPENCL	:= $(SRC_DIR)/opencl

INC_DIRS	:= $(CUDA_PATH)/include
LIB_DIRS	:= $(CUDA_PATH)/$(LIBARCH)
LIBRARIES	:= OpenCL

INC_FLAGS	:= $(patsubst %,-I%, $(INC_DIRS))
C_FLAGS		:= $(patsubst %,-L%, $(LIB_DIRS))
LIB_FLAGS	:= $(patsubst %,-l%, $(LIBRARIES))

all: main-build

clean:
	@rm -rf $(BIN_DIR) $(PTX_DIR)

cuda:
	$(NVCC) --ptx $(SRC_CUDA)/add_vectors.cu -o $(PTX_DIR)/add_vectors_cuda.ptx -gencode=arch=compute_30,code=sm_30 $(INC_FLAGS) $(C_FLAGS) $(LIB_FLAGS)

opencl:
    
	$(CC) -Wall -g $(SRC_OPENCL)/add_vectors.c $(SRC_OPENCL)/opencl_error.c -o $(BIN_DIR)/add_vectors_opencl $(INC_FLAGS) $(C_FLAGS) $(LIB_FLAGS)
	@cp $(SRC_OPENCL)/add_vectors.cl $(KERNEL_DIR)

pre-build:
	@mkdir -p $(BIN_DIR) $(KERNEL_DIR) $(PTX_DIR)

main-build: pre-build
	@$(MAKE) --no-print-directory cuda opencl
