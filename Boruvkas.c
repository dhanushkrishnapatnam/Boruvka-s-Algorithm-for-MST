#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_VERTICES 100
#define MAX_EDGES 100

// Edge structure
typedef struct {
    int source;
    int target;
    int weight;
} Edge;

// Function to find parent of a vertex
int find(int parent[], int i) {
    if (parent[i] == i)
        return i;
    return find(parent, parent[i]);
}

// Function to perform union of two sets
void unionSets(int parent[], int rank[], int x, int y) {
    int xRoot = find(parent, x);
    int yRoot = find(parent, y);

    if (rank[xRoot] < rank[yRoot])
        parent[xRoot] = yRoot;
    else if (rank[xRoot] > rank[yRoot])
        parent[yRoot] = xRoot;
    else {
        parent[yRoot] = xRoot;
        rank[xRoot]++;
    }
}

// Borůvka's algorithm for finding minimum spanning tree
void boruvkaMST(Edge edges[], int numEdges, int numVertices) {
    Edge resultMST[numVertices - 1];
    int parent[numVertices];
    int rank[numVertices];

    // Initialize parent array and rank array
    for (int i = 0; i < numVertices; ++i) {
        parent[i] = i;
        rank[i] = 0;
    }

    int numTrees = numVertices;
    int minWeight[numVertices];
    int cheapest[numVertices];

    // Main loop of Borůvka's algorithm
    while (numTrees > 1) {
        for (int i = 0; i < numVertices; ++i)
            minWeight[i] = -1;

        // Traverse through all edges and update minWeight
        for (int i = 0; i < numEdges; ++i) {
            int srcSet = find(parent, edges[i].source);
            int destSet = find(parent, edges[i].target);

            if (srcSet != destSet) {
                if (minWeight[srcSet] == -1 || edges[minWeight[srcSet]].weight > edges[i].weight)
                    minWeight[srcSet] = i;

                if (minWeight[destSet] == -1 || edges[minWeight[destSet]].weight > edges[i].weight)
                    minWeight[destSet] = i;
            }
        }

        // Add the cheapest edges to MST
        for (int i = 0; i < numVertices; ++i) {
            if (minWeight[i] != -1) {
                int srcSet = find(parent, edges[minWeight[i]].source);
                int destSet = find(parent, edges[minWeight[i]].target);

                if (srcSet != destSet) {
                    resultMST[numVertices - numTrees] = edges[minWeight[i]];
                    unionSets(parent, rank, srcSet, destSet);
                    numTrees--;
                }
            }
        }
    }

    // Print the MST edges
    printf("Minimum Spanning Tree Edges:\n");
    for (int i = 0; i < numVertices - 1; ++i) {
        printf("%d - %d : %d\n", resultMST[i].source, resultMST[i].target, resultMST[i].weight);
    }
}

int main() {
    char filename[100];
    printf("Enter the filename to read input from: ");
    scanf("%s", filename);

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file. Exiting.\n");
        return 1;
    }

    int numVertices, numEdges;
    fscanf(file, "%d %d", &numVertices, &numEdges);

    // Check for invalid input
    if (numVertices <= 0 || numEdges <= 0 || numEdges > numVertices * (numVertices - 1) / 2) {
        printf("Invalid input. Exiting.\n");
        return 1;
    }

    Edge edges[MAX_EDGES];
    for (int i = 0; i < numEdges; ++i) {
        fscanf(file, "%d %d %d", &edges[i].source, &edges[i].target, &edges[i].weight);
    }

    fclose(file);

    // Compute MST using Borůvka's algorithm
    boruvkaMST(edges, numEdges, numVertices);

    return 0;
}
