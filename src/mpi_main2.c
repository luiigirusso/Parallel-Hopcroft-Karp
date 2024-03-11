/*
# Course: High Performance Computing 2022/2023
# 
# Lecturer: Francesco Moscato fmoscato@unisa.it
#
# Student: 
# Russo Luigi  0622702071  l.russo86@studenti.unisa.it
#
# This file is part of HopcroftKarp.
#
# HopcroftKarp is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# HopcroftKarp is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with ParallelTarjan.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#include "generator.c"
#include "hopcroft_karp.c"

int main(int argc, char *argv[]) {
    // Initialize MPI
    MPI_Init(&argc, &argv);

    // Get rank and size of the MPI communicator
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Check if the correct number of arguments is provided
    if (argc != 3) {
        if (rank == 0) {
            printf("Usage: %s <m> <n>\n", argv[0]);
        }
        MPI_Finalize();
        return 1;
    }

    // Extract m and n from command line arguments
    int m = atoi(argv[1]);
    int n = atoi(argv[2]);

    // Ensure that m and n are positive integers
    if (m <= 0 || n <= 0) {
        if (rank == 0) {
            printf("m and n must be positive integers\n");
        }
        MPI_Finalize();
        return 1;
    }

    int **adj = NULL;
    BipartiteGraph *graph = NULL;

    // Process 0 generates the entire bipartite graph
    if (rank == 0) {
        // Allocate memory for the entire bipartite graph
        adj = (int **)malloc(m * sizeof(int *));
        for (int i = 0; i < m; i++) {
            adj[i] = (int *)malloc(n * sizeof(int));
        }

        // Seed the random number generator based on m and n
        srand(m * n);

        // Generate the entire bipartite graph
        generateRandomBipartiteGraph(m, n, adj);

        // Create a bipartite graph with specified number of vertices in sets X and Y
        graph = createBipartiteGraph(m, n);
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (adj[i][j] == 1) {
                    addEdge(graph, i + 1, j + 1);
                }
            }
        }
    }

    // Broadcast graph dimensions to all processes
    MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Allocate separate send and receive buffers for scattering
    int *sendBuffer = NULL;
    int *recvBuffer = (int *)malloc(m * n * sizeof(int));

    // Process 0 prepares the data for scattering
    if (rank == 0) {
        sendBuffer = (int *)malloc(m * n * sizeof(int));
        // Copy the data from adj to sendBuffer
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                sendBuffer[i * n + j] = adj[i][j];
            }
        }
    }

    // Scatter the graph data among all processes
    MPI_Scatter(sendBuffer, m * n, MPI_INT, recvBuffer, m * n, MPI_INT, 0, MPI_COMM_WORLD);

    // Create a local bipartite graph with specified number of vertices in sets X and Y
    BipartiteGraph *localGraph = createBipartiteGraph(m, n);
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (recvBuffer[i * n + j] == 1) {
                addEdge(localGraph, i + 1, j + 1);
            }
        }
    }

    // Execute the Hopcroft-Karp algorithm in parallel on each MPI process
    int localMatching = hopcroftKarp(localGraph);

    // Reduce local matchings to find the global maximum matching
    int globalMatching = 0;
    MPI_Reduce(&localMatching, &globalMatching, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    // Process 0 prints the global maximum matching
    if (rank == 0) {
        printf("The maximum matching is: %d\n", globalMatching);

        // Free memory allocated for the graph
        free(graph->matchX);
        free(graph->matchY);
        free(graph->dist);
        for (int i = 0; i <= m; i++)
            free(graph->adj[i]);
        free(graph->adj);
        free(graph);
        free(sendBuffer);
    }

    // Free memory allocated for the local graph and recvBuffer
    free(localGraph->matchX);
    free(localGraph->matchY);
    free(localGraph->dist);
    for (int i = 0; i <= m; i++)
        free(localGraph->adj[i]);
    free(localGraph->adj);
    free(recvBuffer);

    // Finalize MPI
    MPI_Finalize();

    return 0;
}
