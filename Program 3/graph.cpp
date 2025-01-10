/* Name: Talha Akhlaq
   Description: Constructs a graph from an input file using a hash table and adjacency lists,
   applies Dijkstra's algorithm with a binary heap for shortest path calculation, and outputs results to a file.
*/

#include "graph.h"
#include <iostream>
#include <fstream>
#include <stack>

using namespace std;

// Deallocates memory for dynamically created Vertex objects.
Graph::~Graph()
{
  for (auto &v : vertexList)
  {
    delete v;
  }
}

// Constructor to initialize graph structure from input file.
Graph::Graph(const string &input_file)
    : vertices(100000) // Sets initial hash table size.
{
  loadGraph(input_file);
}

// Reads vertices and edges from file, constructing graph structure.
void Graph::loadGraph(const string &fileName)
{
  ifstream inputFile(fileName);
  if (!inputFile.is_open())
  {
    cerr << "Error: Could not open file " << fileName << endl;
    exit(EXIT_FAILURE);
  }

  string vertex1, vertex2;
  int weight;

  // Parses each line to retrieve vertex pairs and edge weights.
  while (inputFile >> vertex1 >> vertex2 >> weight)
  {
    insertEdge(vertex1, vertex2, weight);
  }
  inputFile.close();
}

// Adds directed edge from source to destination with weight.
void Graph::insertEdge(const string &sourceId, const string &destId, int weight)
{
  Vertex *v1 = getOrCreateVertex(sourceId); // Finds or creates source vertex.
  Vertex *v2 = getOrCreateVertex(destId);   // Finds or creates destination vertex.

  // Connects v1 to v2 with specified weight.
  Edge newEdge = {v2, weight};
  v1->adjList.push_back(newEdge);
}

// Retrieves existing vertex or creates a new one if not found.
Graph::Vertex *Graph::getOrCreateVertex(const string &name)
{
  bool found = false;
  Vertex *v = static_cast<Vertex *>(vertices.getPointer(name, &found));
  if (!found)
  {
    // Inserts new vertex in hash table and stores it in vertex list.
    v = new Vertex(name);     // Create new vertex.
    vertices.insert(name, v); // Add vertex to hash table.
    vertexList.push_back(v);  // Store vertex in list.
  }
  return v;
}

// Checks if vertex exists within the graph.
bool Graph::isValidVertex(const string &vertexId) const
{
  return vertices.contains(vertexId);
}

// Executes Dijkstra's algorithm to calculate shortest paths from source.
void Graph::dijkstra(const string &startVertex)
{
  Vertex *source = static_cast<Vertex *>(vertices.getPointer(startVertex));
  if (!source)
  {
    cerr << "Error: Starting vertex '" << startVertex << "' not found in graph." << endl;
    return;
  }

  // Initializes all vertices to max distance and unprocessed state.
  for (auto &v : vertexList)
  {
    v->distance = INT32_MAX; // Set max distance.
    v->known = false;        // Mark as unprocessed.
    v->previous = nullptr;   // No previous vertex yet.
  }

  source->distance = 0; // Sets source distance to zero.

  // Adds source vertex to the min-heap.
  heap minHeap(vertexList.size());
  int insertStatus = minHeap.insert(source->id, source->distance, source);
  if (insertStatus != 0) // Verifies successful insertion.
  {
    cerr << "Error: Failed to insert source vertex into heap." << endl;
    return;
  }

  // Loop through all vertices in heap to determine shortest paths.
  while (minHeap.currentSize > 0)
  {
    string vertexName;
    int dist;
    Vertex *u = nullptr;

    // Extracts the vertex with minimum distance.
    if (minHeap.deleteMin(&vertexName, &dist, reinterpret_cast<void **>(&u)) != 0)
    {
      cerr << "Error: Failed to delete min from heap." << endl;
      break;
    }

    if (!u) // Skips null vertices.
    {
      cerr << "Error: Extracted null vertex from heap." << endl;
      continue;
    }

    if (u->known) // Skips already processed vertices.
    {
      continue;
    }

    u->known = true; // Marks vertex as processed.

    // Updates neighboring vertices if a shorter path is found.
    for (const auto &edge : u->adjList)
    {
      Vertex *v = edge.destination;
      int newDist = u->distance + edge.weight;

      // Updates vertex with new shortest distance if applicable.
      if (!v->known && newDist < v->distance)
      {
        v->distance = newDist;
        v->previous = u;

        int heapStatus = minHeap.insert(v->id, v->distance, v);
        if (heapStatus == 2) // Adjusts position if vertex is already in heap.
        {
          minHeap.setKey(v->id, v->distance);
        }
        else if (heapStatus != 0) // Checks for insertion failure.
        {
          cerr << "Error: Failed to insert/update vertex " << v->id << " in heap." << endl;
        }
      }
    }
  }
}

// Outputs shortest paths from source to each reachable vertex.
void Graph::outputPaths(const string &outFileName) const
{
  ofstream outFile(outFileName);
  if (!outFile.is_open())
  {
    cerr << "Error: Could not open output file " << outFileName << endl;
    return;
  }

  // Formats and writes each vertex's shortest path and distance.
  for (const auto &v : vertexList)
  {
    outFile << v->id << ": ";
    if (v->distance == INT32_MAX)
    {
      outFile << "NO PATH" << endl;
    }
    else
    {
      outFile << v->distance << " [" << formatPath(v) << "]" << endl;
    }
  }
  outFile.close();
}

// Constructs path from source to specified vertex.
string Graph::formatPath(const Vertex *v) const
{
  stack<string> pathStack;
  const Vertex *current = v;

  // Builds path in reverse using previous pointers.
  while (current)
  {
    pathStack.push(current->id);
    current = current->previous;
  }

  if (pathStack.empty())
  {
    return "";
  }

  // Formats path by popping elements from stack.
  string path = pathStack.top();
  pathStack.pop();
  while (!pathStack.empty())
  {
    path += ", " + pathStack.top();
    pathStack.pop();
  }
  return path;
}
