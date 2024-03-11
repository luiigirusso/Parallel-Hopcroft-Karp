CC=mpicc
GCC=gcc
NVCC=nvcc
MPI_INCLUDE=/usr/lib/x86_64-linux-gnu/mpich/include
CUDA_INCLUDE=/usr/local/cuda/include
CUDA_LIB=/usr/local/cuda/lib64
CFLAGS=-O3 -I./include -I$(MPI_INCLUDE)
GFLAGS=-O3 -I./include -I$(MPI_INCLUDE)
NVCCFLAGS=-O3 -I./include -I$(MPI_INCLUDE) -I$(CUDA_INCLUDE)
LDFLAGS=-L$(MPI_LIB) -L$(CUDA_LIB)
LIBS=-lmpi -lcudart

all: mpi sequential cuda cuda_mpi

mpi: ./src/mpi_main.c 
	$(CC) $(CFLAGS) -o ./bin/mpi ./src/mpi_main.c 

sequential: ./src/sequential_main.c 
	$(GCC) $(GFLAGS) -o ./bin/sequential ./src/sequential_main.c

cuda: ./src/cuda_main.cu
	$(NVCC) $(NVCCFLAGS) $(LDFLAGS) -o ./bin/cuda ./src/cuda_main.cu $(LIBS)

cuda_mpi: ./src/cuda_mpi_main.cu
	$(NVCC) $(NVCCFLAGS) $(LDFLAGS) -o ./bin/cuda_mpi ./src/cuda_mpi_main.cu $(LIBS)

clean:
	rm -f ./bin/mpi ./bin/sequential ./bin/cuda ./bin/cuda_mpi
