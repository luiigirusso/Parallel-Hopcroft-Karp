#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>




// Funzione per generare un numero casuale compreso tra min e max
int randomInt(int min, int max) {
    return min + rand() % (max - min + 1);
}

// Funzione per generare un grafo bipartito casuale con m vertici nella prima partizione e n vertici nella seconda partizione
void generateRandomBipartiteGraph(int m, int n, int **adj) {
    //srand(time(NULL)); // Inizializzazione del seme casuale

    // Inizializzazione della matrice di adiacenza
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            adj[i][j] = 0;
        }
    }

    // Generazione delle connessioni tra i vertici delle due partizioni
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            // Generazione casuale di un valore booleano per decidere se creare un arco tra i vertici i e j
            bool connect = rand() % 2;
            if (connect) {
                adj[i][j] = 1;
            }
        }
    }
}

// Funzione per stampare il grafo bipartito
void printBipartiteGraph(int m, int n, int **adj) {
    printf("Matrice di adiacenza:\n");
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", adj[i][j]);
        }
        printf("\n");
    }
}