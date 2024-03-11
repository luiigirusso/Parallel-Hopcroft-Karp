#include "hopcroft_karp.c"

#define MAX_NODES 1000

/*
 * Kernel Function: cuda_bfs
 * -------------------------
 * Performs breadth-first search (BFS) on the bipartite graph represented as an adjacency matrix
 * to find augmenting paths for the Hopcroft-Karp algorithm using CUDA parallelism.
 * 
 * graph: Pointer to the adjacency matrix of the bipartite graph (device memory).
 * matchX: Array storing the matched vertices in set X (device memory).
 * matchY: Array storing the matched vertices in set Y (device memory).
 * queue: Array used as a queue for BFS traversal (device memory).
 * visited: Array to mark visited vertices during BFS traversal (device memory).
 * level: Array to store the level of each vertex during BFS traversal (device memory).
 * m: Pointer to the number of vertices in set X (device memory).
 * n: Pointer to the number of vertices in set Y (device memory).
 */
__global__ void cuda_bfs(int *graph, int *matchX, int *matchY, int *queue, int *visited, int *level, int *m, int *n) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx < *m) {
        int u = idx + 1;
        if (matchX[u] == -1) {
            level[u] = 0;
            queue[0] = u;
            int front = 0, rear = 1;
            while (front < rear) {
                int v = queue[front++];
                for (int i = 0; i < *n; ++i) {
                    if (graph[u * (*n) + i] && matchY[i + 1] == -1) {
                        matchX[u] = i + 1;
                        matchY[i + 1] = u;
                        return;
                    }
                    if (graph[u * (*n) + i] && level[matchY[i + 1]] == -1) {
                        level[matchY[i + 1]] = level[u] + 1;
                        queue[rear++] = matchY[i + 1];
                    }
                }
            }
        }
    }
}


/*
 * Function: hopcroftKarp_cuda
 * ----------------------------
 * Applies the Hopcroft-Karp algorithm for finding maximum matchings in a bipartite graph using CUDA.
 * 
 * graph: Pointer to the bipartite graph structure.
 * 
 * returns: The number of edges in the maximum matching.
 */
int hopcroftKarp_cuda(BipartiteGraph *graph) {
    // CUDA device memory allocation
    int *d_graph, *d_matchX, *d_matchY, *d_queue, *d_visited, *d_level, *d_m, *d_n;
    cudaMalloc((void **)&d_graph, (graph->m + 1) * (graph->n + 1) * sizeof(int));
    cudaMalloc((void **)&d_matchX, (graph->m + 1) * sizeof(int));
    cudaMalloc((void **)&d_matchY, (graph->n + 1) * sizeof(int));
    cudaMalloc((void **)&d_queue, (graph->m + 1) * sizeof(int));
    cudaMalloc((void **)&d_visited, (graph->n + 1) * sizeof(int));
    cudaMalloc((void **)&d_level, (graph->m + 1) * sizeof(int));
    cudaMalloc((void **)&d_m, sizeof(int));
    cudaMalloc((void **)&d_n, sizeof(int));

    // Copy graph data to device memory
    cudaMemcpy(d_graph, graph, (graph->m + 1) * (graph->n + 1) * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(d_m, &(graph->m), sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(d_n, &(graph->n), sizeof(int), cudaMemcpyHostToDevice);

    // Define CUDA kernel parameters
    int block_size = 256;
    int num_blocks = (graph->m + block_size - 1) / block_size;

    // Launch CUDA BFS kernel
    cuda_bfs<<<num_blocks, block_size>>>(d_graph, d_matchX, d_matchY, d_queue, d_visited, d_level, d_m, d_n);

    // Copy matching results back to host memory
    cudaMemcpy(graph->matchX, d_matchX, (graph->m + 1) * sizeof(int), cudaMemcpyDeviceToHost);
    cudaMemcpy(graph->matchY, d_matchY, (graph->n + 1) * sizeof(int), cudaMemcpyDeviceToHost);

    // Free device memory
    cudaFree(d_graph);
    cudaFree(d_matchX);
    cudaFree(d_matchY);
    cudaFree(d_queue);
    cudaFree(d_visited);
    cudaFree(d_level);
    cudaFree(d_m);
    cudaFree(d_n);

    // Calculate the number of edges in the matching
    int matching = 0;
    for (int u = 1; u <= graph->m; u++) {
        if (graph->matchX[u] != -1)
            matching++;
    }

    return matching;
}