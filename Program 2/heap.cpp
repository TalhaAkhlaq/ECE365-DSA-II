/* Name: Talha Akhlaq
   Implements a binary heap using an array for structure and a hash table for lookup by ID.
   Provides operations for inserting nodes, updating keys, removing nodes, and retrieving the minimum element.
   Percolation methods maintain heap order during insertion and deletion.
*/

#include "heap.h"
#include <climits>
#include <iostream>

using namespace std;

// Constructor for initializing heap with a given capacity
heap::heap(int capacity)
{
  this->capacity = capacity;         // Set the capacity of the heap
  this->currentSize = 0;             // Initialize the heap size to zero
  data.resize(capacity + 1);         // Allocate space for heap items (1-based indexing)
  mapping = hashTable(capacity * 2); // Initialize hash table for quick lookups
}

// Insert a node into the heap with the specified ID and key
int heap::insert(const std::string &id, int key, void *pv)
{
  if (currentSize >= capacity)
  {
    return 1;
  }

  if (mapping.contains(id))
  {
    return 2;
  }

  currentSize++;
  data[currentSize].id = id;
  data[currentSize].key = key;
  data[currentSize].pData = pv;

  if (mapping.insert(id, &data[currentSize]) != 0)
  {
    return 3; // Error code for hash table insertion failure
  }

  percolateUp(currentSize);
  return 0;
}

// Set the key of the node with the given ID and adjust the heap accordingly
int heap::setKey(const std::string &id, int key)
{
  // Check if the node exists in the heap
  if (!mapping.contains(id))
  {
    return 1;
  }

  // Retrieve the node's pointer from the hash table
  node *pn = static_cast<node *>(mapping.getPointer(id));
  int oldKey = pn->key;
  pn->key = key;
  int pos = getPos(pn); // Get the position of the node in the heap

  // Perform percolation based on whether the key was increased or decreased
  if (key > oldKey)
  {
    percolateDown(pos);
  }
  else
  {
    percolateUp(pos);
  }

  return 0;
}

// Delete the node with the smallest key (the root of the heap) and return its details
int heap::deleteMin(std::string *pId, int *pKey, void **ppData)
{
  if (currentSize == 0)
  {
    return 1;
  }

  if (pId != nullptr) // If pId is provided, assign the ID of the root
  {
    *pId = data[1].id;
  }
  if (pKey != nullptr) // If pKey is provided, assign the key of the root
  {
    *pKey = data[1].key;
  }
  if (ppData != nullptr) // If ppData is provided, assign the data pointer of the root
  {
    *ppData = data[1].pData;
  }

  mapping.remove(data[1].id); // Remove the root's ID from the hash table

  data[1] = data[currentSize--]; // Replace the root with the last element and decrease the heap size

  percolateDown(1); // Restore heap order by percolating the new root down

  return 0;
}

// Remove the node with the specified ID from the heap and return its details
int heap::remove(const std::string &id, int *pKey, void **ppData)
{
  if (!mapping.contains(id)) // Check if the node with the given ID exists in the heap
  {
    return 1;
  }

  node *pn = static_cast<node *>(mapping.getPointer(id)); // Get the pointer to the node with the specified ID
  int pos = getPos(pn);                                   // Get the position of the node in the heap

  if (pKey != nullptr) // If pKey is provided, store the node's key
  {
    *pKey = pn->key;
  }
  if (ppData != nullptr) // If ppData is provided, store the node's associated data
  {
    *ppData = pn->pData;
  }

  setKey(id, INT_MIN); // Set the node's key to the minimum value to bring it to the root
  deleteMin();         // Delete the minimum node (which is now the node to be removed)

  return 0;
}

// Percolate a node up the heap to restore the heap property
void heap::percolateUp(int posCur)
{
  node tmp = data[posCur]; // Store the node being percolated

  // Move the node up the heap as long as it's smaller than its parent
  for (; posCur > 1 && tmp.key < data[posCur / 2].key; posCur /= 2)
  {
    data[posCur] = data[posCur / 2];
    mapping.setPointer(data[posCur].id, &data[posCur]); // Update hash table
  }

  // Place the node in its correct position
  data[posCur] = tmp;
  mapping.setPointer(tmp.id, &data[posCur]);
}

// Percolate a node down the heap to restore the heap property
void heap::percolateDown(int posCur)
{
  node tmp = data[posCur]; // Store the node being percolated

  // Move the node down the heap as long as it's larger than its smaller child
  int child;
  for (; posCur * 2 <= currentSize; posCur = child)
  {
    child = posCur * 2;

    // Choose the smaller of the two children
    if (child != currentSize && data[child + 1].key < data[child].key)
    {
      child++;
    }

    // If the node is smaller than the smaller child, stop percolating
    if (tmp.key <= data[child].key)
    {
      break;
    }

    data[posCur] = data[child];
    mapping.setPointer(data[posCur].id, &data[posCur]); // Update hash table
  }

  // Place the node in its correct position
  data[posCur] = tmp;
  mapping.setPointer(tmp.id, &data[posCur]);
}

// Get the position of a node in the heap array
int heap::getPos(node *pn)
{
  return pn - &data[0];
}
