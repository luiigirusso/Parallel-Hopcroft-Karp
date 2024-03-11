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
#include <stdbool.h>
#include <time.h>
#include "hopcroft_karp.c"
#include "generator.c"

int main(int argc, char *argv[]) {
    // Check if the correct number of arguments is provided
    if (argc != 3) {
        printf("Usage: %s <m> <n>\n", argv[0]);
        return 1;
    }

    // Extract m and n from command line arguments
    int m = atoi(argv[1]);
    int n = atoi(argv[2]);

    // Ensure that m and n are positive integers
    if (m <= 0 || n <= 0) {
        printf("m and n must be positive integers\n");
        return 1;
    }
    
    // Allocate memory for the adjacency matrix
    int **adj = (int **)malloc(m * sizeof(int *));
    for (int i = 0; i < m; i++) {
        adj[i] = (int *)malloc(n * sizeof(int));
    }

    // Fix the random seed based on the combination of m and n
    srand(m * n);

    // Generate a random bipartite graph
    generateRandomBipartiteGraph(m, n, adj);

    // Create a bipartite graph with specified number of vertices in sets X and Y
    BipartiteGraph *graph = createBipartiteGraph(m, n);

    // Convert the randomly generated adjacency matrix into the format required by the hopcroft_karp library
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (adj[i][j] == 1) {
                addEdge(graph, i + 1, j + 1); // Add edges, note that indices in C start from 0, while the Hopcroft-Karp library starts from 1
            }
        }
    }

    // Execute the Hopcroft-Karp algorithm to find the maximum matching
    int maxMatching = hopcroftKarp(graph);

    // Print the result
    printf("The maximum matching is: %d\n", maxMatching);

    // Free memory allocated for the graph and adjacency matrix
    for (int i = 0; i < m; i++) {
        free(adj[i]);
    }
    free(adj);
    destroyGraph(graph);

    return 0;
}
