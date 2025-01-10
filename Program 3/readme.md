# Program 3: Dijkstra’s Algorithm

## Description

This program calculates the shortest paths from a starting vertex to all other vertices in a graph using Dijkstra’s algorithm, leveraging a binary heap for efficient priority queue operations.

## Files

- **Dijkstra.cpp**: Implements Dijkstra’s algorithm and handles graph input/output.
- **Heap.cpp** & **Heap.h**: Binary heap used for priority queue operations.
- **Hash.cpp** & **Hash.h**: Hash table for mapping vertex IDs to graph nodes.


## Functionality
1. Reads graph data from an input file where edges are defined by source, destination, and weight.
2. Computes shortest paths using a binary heap for efficient edge relaxation.
3. Outputs distances and paths to all vertices or indicates "NO PATH" if unreachable.
