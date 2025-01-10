#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <list>
#include <climits>
#include "heap.h"
#include "hash.h"

using namespace std;

class Graph
{
public:
    // Initializes the graph by loading from the specified input file.
    Graph(const string &input_file);

    // Cleans up dynamically allocated Vertex objects.
    ~Graph();

    // Runs Dijkstra's algorithm from the specified starting vertex.
    void dijkstra(const string &startVertex);

    // Outputs shortest paths from the starting vertex to each vertex in the graph to a file.
    void outputPaths(const string &outFileName) const;

    // Checks if a vertex with the given ID exists in the graph.
    bool isValidVertex(const string &vertexId) const;

private:
    struct Vertex; // Forward declaration for Vertex usage within Edge struct.

    // Represents a directed edge with a weight.
    struct Edge
    {
        int weight;          // Edge weight.
        Vertex *destination; // Pointer to destination vertex.

        Edge(Vertex *dest, int w) : destination(dest), weight(w) {} // Initializes edge.
    };

    // Represents a node in the graph.
    struct Vertex
    {
        string id;          // Vertex ID.
        int distance;       // Known shortest distance from source.
        bool known;         // Processed flag for Dijkstra's.
        Vertex *previous;   // Previous vertex in the shortest path.
        list<Edge> adjList; // Outgoing edges.

        Vertex(const string &vertexId)
            : id(vertexId), distance(INT32_MAX), known(false), previous(nullptr) {}
    };

    hashTable vertices;        // Hash table for vertex lookup by ID.
    list<Vertex *> vertexList; // Maintains vertex insertion order.

    // Loads graph structure from the specified file.
    void loadGraph(const string &fileName);

    // Inserts a weighted edge between two vertices.
    void insertEdge(const string &sourceId, const string &destId, int weight);

    // Retrieves or creates a vertex for the given name.
    Vertex *getOrCreateVertex(const string &name);

    // Formats the path from source to a given vertex as a string.
    string formatPath(const Vertex *v) const;
};

#endif // GRAPH_H
