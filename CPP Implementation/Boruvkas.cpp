#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

// Edge structure
struct Edge {
    int source;
    int target;
    int weight;
};

// Function to find parent of a vertex
int find(vector<int>& parent, int i) {
    if (parent[i] == i)
        return i;
    return find(parent, parent[i]);
}

// Function to perform union of two sets
void unionSets(vector<int>& parent, vector<int>& rank, int x, int y) {
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
void boruvkaMST(vector<Edge>& edges, int numVertices) {
    vector<Edge> resultMST;
    vector<int> parent(numVertices);
    vector<int> rank(numVertices);

    // Initialize parent array and rank array
    for (int i = 0; i < numVertices; ++i) {
        parent[i] = i;
        rank[i] = 0;
    }

    int numTrees = numVertices;
    vector<int> minWeight(numVertices, -1);

    // Main loop of Borůvka's algorithm
    while (numTrees > 1) {
        // Traverse through all edges and update minWeight
        for (const Edge& edge : edges) {
            int srcSet = find(parent, edge.source);
            int destSet = find(parent, edge.target);

            if (srcSet != destSet) {
                if (minWeight[srcSet] == -1 || edges[minWeight[srcSet]].weight > edge.weight)
                    minWeight[srcSet] = edge.source;

                if (minWeight[destSet] == -1 || edges[minWeight[destSet]].weight > edge.weight)
                    minWeight[destSet] = edge.target;
            }
        }

        // Add the cheapest edges to MST
        for (int i = 0; i < numVertices; ++i) {
            if (minWeight[i] != -1) {
                int srcSet = find(parent, edges[minWeight[i]].source);
                int destSet = find(parent, edges[minWeight[i]].target);

                if (srcSet != destSet) {
                    resultMST.push_back(edges[minWeight[i]]);
                    unionSets(parent, rank, srcSet, destSet);
                    numTrees--;
                }
            }
        }
    }

    // Print the MST edges
    cout << "Minimum Spanning Tree Edges:" << endl;
    for (const Edge& edge : resultMST) {
        cout << edge.source << " - " << edge.target << " : " << edge.weight << endl;
    }
}

int main() {
    string filename;
    cout << "Enter the filename to read input from: ";
    cin >> filename;

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file. Exiting." << endl;
        return 1;
    }

    int numVertices, numEdges;
    file >> numVertices >> numEdges;

    // Check for invalid input
    if (numVertices <= 0 || numEdges <= 0 || numEdges > numVertices * (numVertices - 1) / 2) {
        cout << "Invalid input. Exiting." << endl;
        return 1;
    }

    vector<Edge> edges(numEdges);
    for (int i = 0; i < numEdges; ++i) {
        file >> edges[i].source >> edges[i].target >> edges[i].weight;
    }

    file.close();

    // Compute MST using Borůvka's algorithm
    boruvkaMST(edges, numVertices);

    return 0;
}
