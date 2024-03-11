# Parallel version of Hopcroft-Karp algorithm

Project description: 
Make a parallel MPI and CUDA version for the Hopcroft-Karp algorithm for solving the Graph Matching problem:

https://en.wikipedia.org/wiki/Hopcroft%E2%80%93Karp_algorithm

https://www.cs.princeton.edu/courses/archive/spr11/cos423/Lectures/GraphMatching.pdf

https://www.cs.cmu.edu/~15850/notes/lec7.pdf

## Requirements

+ C compiler supporting MPI and CUDA

## Compilation

To compile the project use the provided makefile. 
Open a terminal window and navigate to the project directory, then run the following commands:

```bash
make clean
make
```

## Execution

To execute the project, use the following commands:

(m: Number of vertices in set X)

(n: Number of vertices in set Y)

**Sequential version**

```bash
./bin/sequential <m> <n>
```

**MPI version**

```bash
mpirun -np 4 ./bin/mpi <m> <n>
```

**CUDA version**

```bash
./bin/cuda <m> <n>
```

**CUDA+MPI version**

```bash
mpirun -np 4 ./bin/cuda_mpi <m> <n>
```

To perform a test of correct operation with default values of m and n, use the following command:

```bash
./test.sh
```

N.B. The default values in this case are 10 and 10, but can be changed as desired.



## License

This project is released under the GNU General Public Licence. Please read the LICENSE file for more details.
