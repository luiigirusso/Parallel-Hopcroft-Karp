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
#include <limits.h>
#include <string.h>
#include "graph.c"

/*
 * Function: hopcroftKarp
 * ------------------------
 * Finds the maximum matching in the given bipartite graph.
 * 
 * graph: the bipartite graph to perform the algorithm on.
 * 
 * returns: the number of matchings found.
 */
int hopcroftKarp(BipartiteGraph *graph) {
    memset(graph->matchX, NIL, (graph->m + 1) * sizeof(int));
    memset(graph->matchY, NIL, (graph->n + 1) * sizeof(int));
    int matching = 0;
    while (bfs(graph)) {
        for (int u = 1; u <= graph->m; u++) {
            if (graph->matchX[u] == NIL && dfs(graph, u))
                matching++;
        }
    }
    return matching;
}

/*
 * Function: printMatching
 * ------------------------
 * Prints the matching found in the bipartite graph.
 * 
 * graph: the bipartite graph containing the matching.
 */
void printMatching(BipartiteGraph *graph) {
    printf("Matching:\n");
    for (int u = 1; u <= graph->m; u++) {
        if (graph->matchX[u] != NIL) {
            printf("%d - %d\n", u, graph->matchX[u]);
        }
    }
}
