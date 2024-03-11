#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#include "hopcroft_karp.c"
#include "generator.c"



int main(int argc, char *argv[]) {
    // Initialize MPI
    MPI_Init(&argc, &argv);

    // Get rank and size of the MPI communicator
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

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
    if (rank == 0) {
        printBipartiteGraph(m, n, adj);
    }

    // Execute the Hopcroft-Karp algorithm in parallel on each MPI process
    int localMatching = hopcroftKarp(graph);

    // Gather local matchings from all MPI processes to rank 0
    int *allMatchings = NULL;
    int globalMatching = 0;
    if (rank == 0) {
        allMatchings = (int *)malloc(size * sizeof(int));
    }
    MPI_Gather(&localMatching, 1, MPI_INT, allMatchings, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Find the global maximum matching from all local matchings
    if (rank == 0) {
        for (int i = 0; i < size; i++) {
            if (allMatchings[i] > globalMatching) {
                globalMatching = allMatchings[i];
            }
        }
        // Print the global maximum matching
        printf("The maximum matching is: %d\n", globalMatching);
        printMatching(graph);
        free(allMatchings);
    }

    // Free memory allocated for the graph
    free(graph->matchX);
    free(graph->matchY);
    free(graph->dist);
    for (int i = 0; i <= m; i++)
        free(graph->adj[i]);
    free(graph->adj);
    free(graph);

    // Finalize MPI
    MPI_Finalize();

    return 0;
}
