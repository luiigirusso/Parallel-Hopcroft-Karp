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
#include <limits.h>


#define NIL 0
#define INF INT_MAX

// Structure to represent a Bipartite Graph
typedef struct {
    int V;          // Total number of vertices
    int m;          // Number of vertices in set X
    int n;          // Number of vertices in set Y
    int *matchX;    // Array to store matching from X to Y
    int *matchY;    // Array to store matching from Y to X
    int *dist;      // Array to store distances for BFS
    int **adj;      // Adjacency matrix representation
} BipartiteGraph;

/*
 * Function: createBipartiteGraph
 * -------------------------------
 * Creates a bipartite graph with the specified number of vertices in set X and set Y.
 * 
 * m: the number of vertices in set X.
 * n: the number of vertices in set Y.
 * 
 * returns: a pointer to the created bipartite graph.
 */
BipartiteGraph* createBipartiteGraph(int m, int n) {
    BipartiteGraph *graph = (BipartiteGraph*)malloc(sizeof(BipartiteGraph));
    graph->V = m + n + 1;
    graph->m = m;
    graph->n = n;
    graph->matchX = (int*)malloc((m + 1) * sizeof(int));
    graph->matchY = (int*)malloc((n + 1) * sizeof(int));
    graph->dist = (int*)malloc((m + 1) * sizeof(int));

    for (int i = 0; i <= m; i++)
        graph->matchX[i] = NIL;
    for (int j = 0; j <= n; j++)
        graph->matchY[j] = NIL;

    graph->adj = (int**)malloc((m + 1) * sizeof(int*));
    for (int i = 0; i <= m; i++) {
        graph->adj[i] = (int*)malloc((n + 1) * sizeof(int));
        for (int j = 0; j <= n; j++)
            graph->adj[i][j] = 0;
    }

    return graph;
}

/*
 * Function: addEdge
 * -----------------
 * Adds an edge between vertex x in set X and vertex y in set Y.
 * 
 * graph: the bipartite graph to add the edge to.
 * x: the vertex in set X.
 * y: the vertex in set Y.
 */
void addEdge(BipartiteGraph *graph, int x, int y) {
    graph->adj[x][y] = 1;
}

/*
 * Function: bfs
 * -------------
 * Performs Breadth First Search (BFS) starting from unmatched vertices in set X.
 * 
 * graph: the bipartite graph to perform the BFS on.
 * 
 * returns: true if there exists an augmenting path, false otherwise.
 */
bool bfs(BipartiteGraph *graph) {
    int u, v;
    int queue[graph->V];
    int front = 0, rear = 0;

    for (u = 1; u <= graph->m; u++) {
        if (graph->matchX[u] == NIL) {
            graph->dist[u] = 0;
            queue[rear++] = u;
        } else {
            graph->dist[u] = INF;
        }
    }
    graph->dist[NIL] = INF;

    while (front < rear) {
        u = queue[front++];
        if (u != NIL) {
            for (v = 1; v <= graph->n; v++) {
                if (graph->adj[u][v] && graph->dist[graph->matchY[v]] == INF) {
                    graph->dist[graph->matchY[v]] = graph->dist[u] + 1;
                    queue[rear++] = graph->matchY[v];
                }
            }
        }
    }
    return (graph->dist[NIL] != INF);
}

/*
 * Function: dfs
 * -------------
 * Performs Depth First Search (DFS) for finding augmenting path.
 * 
 * graph: the bipartite graph to perform the DFS on.
 * u: the current vertex being explored.
 * 
 * returns: true if an augmenting path is found, false otherwise.
 */
bool dfs(BipartiteGraph *graph, int u) {
    if (u != NIL) {
        for (int v = 1; v <= graph->n; v++) {
            if (graph->adj[u][v] && graph->dist[graph->matchY[v]] == graph->dist[u] + 1) {
                if (dfs(graph, graph->matchY[v])) {
                    graph->matchX[u] = v;
                    graph->matchY[v] = u;
                    return true;
                }
            }
        }
        graph->dist[u] = INF;
        return false;
    }
    return true;
}

/*
 * Function: destroyGraph
 * ----------------------
 * Destroys the bipartite graph and frees allocated memory.
 * 
 * graph: the bipartite graph to destroy.
 */
void destroyGraph(BipartiteGraph *graph) {
    free(graph->matchX);
    free(graph->matchY);
    free(graph->dist);
    for (int i = 0; i <= graph->m; ++i)
        free(graph->adj[i]);
    free(graph->adj);
    free(graph);
}

