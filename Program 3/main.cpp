/*
   Name: Talha Akhlaq
   Description: Loads a graph from an input file, prompts the user for a starting vertex,
   applies Dijkstra's algorithm to compute shortest paths, and outputs the results to a file.
*/

#include "graph.h"
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace std::chrono;

int main()
{
  string inputFile, outputFile, startVertex;

  // Prompt the user to enter the name of the graph file.
  cout << "Enter the name of the graph file: ";
  cin >> inputFile;

  // Attempt to open the input file; loop until a valid file is provided.
  ifstream input(inputFile);
  while (!input)
  {
    cerr << "Error: Could not open file \"" << inputFile << "\". Please enter a valid graph file: ";
    cin >> inputFile;
    input.clear();
    input.open(inputFile);
  }
  input.close();

  // Initialize graph using the input file.
  Graph g(inputFile);

  // Prompt user for a valid starting vertex ID.
  cout << "Enter name of starting vertex: ";
  cin >> startVertex;
  while (!g.isValidVertex(startVertex))
  {
    cerr << "Error: Invalid vertex ID \"" << startVertex << "\". Please enter a valid vertex ID: ";
    cin >> startVertex;
  }

  // Measure time taken to apply Dijkstra's algorithm.
  auto startTime = steady_clock::now();
  g.dijkstra(startVertex);
  auto endTime = steady_clock::now();
  auto elapsedTime = duration_cast<duration<double>>(endTime - startTime);

  cout << fixed << setprecision(9); // Format output to show up to nine decimal places
  cout << "Total time (in seconds) to apply Dijkstra's algorithm: " << elapsedTime.count() << " seconds" << endl;

  // Prompt user for the name of the output file.
  cout << "Enter the name of the output file: ";
  cin >> outputFile;

  // Write the shortest paths to the specified output file.
  g.outputPaths(outputFile);

  return 0;
}