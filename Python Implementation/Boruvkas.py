class Edge:
    def __init__(self, source, target, weight):
        self.source = source
        self.target = target
        self.weight = weight

def find(parent, i):
    if parent[i] == i:
        return i
    return find(parent, parent[i])

def unionSets(parent, rank, x, y):
    xRoot = find(parent, x)
    yRoot = find(parent, y)

    if rank[xRoot] < rank[yRoot]:
        parent[xRoot] = yRoot
    elif rank[xRoot] > rank[yRoot]:
        parent[yRoot] = xRoot
    else:
        parent[yRoot] = xRoot
        rank[xRoot] += 1

def boruvkaMST(edges, numVertices):
    resultMST = []
    parent = list(range(numVertices))
    rank = [0] * numVertices

    numTrees = numVertices
    minWeight = [-1] * numVertices

    while numTrees > 1:
        for edge in edges:
            srcSet = find(parent, edge.source)
            destSet = find(parent, edge.target)

            if srcSet != destSet:
                if minWeight[srcSet] == -1 or edges[minWeight[srcSet]].weight > edge.weight:
                    minWeight[srcSet] = edge.source

                if minWeight[destSet] == -1 or edges[minWeight[destSet]].weight > edge.weight:
                    minWeight[destSet] = edge.target

        for i in range(numVertices):
            if minWeight[i] != -1:
                srcSet = find(parent, edges[minWeight[i]].source)
                destSet = find(parent, edges[minWeight[i]].target)

                if srcSet != destSet:
                    resultMST.append(edges[minWeight[i]])
                    unionSets(parent, rank, srcSet, destSet)
                    numTrees -= 1

    print("Minimum Spanning Tree Edges:")
    for edge in resultMST:
        print(edge.source, "-", edge.target, ":", edge.weight)

if __name__ == "__main__":
    filename = input("Enter the filename to read input from: ")

    try:
        with open(filename, 'r') as file:
            numVertices, numEdges = map(int, file.readline().split())

            if numVertices <= 0 or numEdges <= 0 or numEdges > numVertices * (numVertices - 1) / 2:
                print("Invalid input. Exiting.")
                exit(1)

            edges = []
            for _ in range(numEdges):
                source, target, weight = map(int, file.readline().split())
                edges.append(Edge(source, target, weight))

        # Compute MST using Borůvka's algorithm
        boruvkaMST(edges, numVertices)

    except FileNotFoundError:
        print("Error opening file. Exiting.")
        exit(1)
