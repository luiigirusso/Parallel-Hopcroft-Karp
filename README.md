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
```make clean```
```make```

## Execution

To execute the project, use the following commands:
**Sequential version**
```./bin/sequential```
**MPI version**
```mpirun -np 4 ./bin/mpi```
**CUDA version**
```./bin/cuda```
**CUDA+MPI version**
```mpirun -np 4 ./bin/cuda_mpi```

## License

This project is released under the GNU General Public Licence. Please read the LICENSE file for more details.
