#ifndef _HEAP_H
#define _HEAP_H

#include <vector>
#include <string>
#include "hash.h"

class heap
{
public:
  // Constructor: sets capacity of heap
  heap(int capacity);

  /// Inserts a node with key, optional data pointer, and id
  // Returns 0 on success, 1 if heap is full, 2 if id already exists, 3 if hash table insert fails
  int insert(const std::string &id, int key, void *pv = nullptr);

  /// Updates key of node by id
  // Returns 0 on success, 1 if id not found
  int setKey(const std::string &id, int key);

  // Deletes the node with the smallest key
  // Optionally writes id, key, and data of the deleted node
  // Returns 0 on success, 1 if heap is empty
  int deleteMin(std::string *pId = nullptr, int *pKey = nullptr, void **ppData = nullptr);

  // Removes node by id and optionally writes its key and data
  // Returns 0 on success, 1 if id not found
  int remove(const std::string &id, int *pKey = nullptr, void **ppData = nullptr);

private:
  // Defines a node in the heap
  class node
  {
  public:
    std::string id; // Node id
    int key;        // Node key
    void *pData;    // Pointer to node's associated data
  };

  int capacity;           // Max heap size
  int currentSize;        // Current number of elements
  std::vector<node> data; // Binary heap storage
  hashTable mapping;      // Hash table for id lookups

  // Moves node at posCur up the heap
  void percolateUp(int posCur);

  // Moves node at posCur down the heap
  void percolateDown(int posCur);

  // Returns the position of the node in the heap array
  int getPos(node *pn);
};

#endif