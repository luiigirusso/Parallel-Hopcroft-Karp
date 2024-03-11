#include <stdio.h>
#include <stdlib.h>
#include "hopcroft_karp.cu"
#include "generator.c"



int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <m> <n>\n", argv[0]);
        return 1;
    }

    int m = atoi(argv[1]);
    int n = atoi(argv[2]);

    if (m <= 0 || n <= 0) {
        printf("m and n must be positive integers\n");
        return 1;
    }
    // Allocazione della matrice di adiacenza
    int **adj = (int **)malloc(m * sizeof(int *));
    for (int i = 0; i < m; i++) {
        adj[i] = (int *)malloc(n * sizeof(int));
    }

    // Fissa il seme casuale in base alla combinazione di m e n
    srand(m * n);

    // Generazione del grafo bipartito casuale
    generateRandomBipartiteGraph(m, n, adj);

    // Create a bipartite graph with specified number of vertices in sets X and Y
    BipartiteGraph *graph = createBipartiteGraph(m, n);

    // Convertire la matrice di adiacenza generata casualmente nel formato richiesto dalla libreria hopcroft_karp
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (adj[i][j] == 1) {
                addEdge(graph, i + 1, j + 1); // Aggiungi archi, ricorda che gli indici in C partono da 0, mentre la libreria Hopcroft-Karp parte da 1
            }
        }
    }

    // Stampa del grafo bipartito
    printBipartiteGraph(m, n, adj);

    // Execute the Hopcroft-Karp algorithm in parallel on the GPU
    int maxMatching = hopcroftKarp(graph);

    // Print the result
    printf("The maximum matching is: %d\n", maxMatching);
    printMatching(graph);

    // Free memory allocated for the graph
    destroyGraph(graph);

    return 0;
}
