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

// Function to generate a random number between min and max
int randomInt(int min, int max) {
    return min + rand() % (max - min + 1);
}

/*
 * Function: generateRandomBipartiteGraph
 * ----------------------------------------
 * Generates a random bipartite graph with m vertices in the first partition and n vertices in the second partition.
 * 
 * m: the number of vertices in the first partition (set X).
 * n: the number of vertices in the second partition (set Y).
 * adj: a 2D array representing the adjacency matrix of the bipartite graph.
 */
void generateRandomBipartiteGraph(int m, int n, int **adj) {
    // srand(time(NULL)); // Initialize the random seed

    // Initialize the adjacency matrix
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            adj[i][j] = 0;
        }
    }

    // Generate connections between vertices of the two partitions
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            // Generate a random boolean value to decide whether to create an edge between vertices i and j
            bool connect = rand() % 2;
            if (connect) {
                adj[i][j] = 1;
            }
        }
    }
}

/*
 * Function: printBipartiteGraph
 * -----------------------------
 * Prints the adjacency matrix of a bipartite graph.
 * 
 * m: the number of vertices in the first partition.
 * n: the number of vertices in the second partition.
 * adj: the adjacency matrix of the bipartite graph.
 */
void printBipartiteGraph(int m, int n, int **adj) {
    printf("Adjacency Matrix:\n");
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", adj[i][j]);
        }
        printf("\n");
    }
}