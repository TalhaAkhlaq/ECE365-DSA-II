#ifndef _HEAP_H
#define _HEAP_H

#include <vector>
#include <string>
#include "hash.h"

using namespace std;

class heap
{
public:
    // Constructor: Initializes the heap with the specified capacity.
    heap(int capacity);

    // Destructor: Cleans up any dynamically allocated resources.
    ~heap();

    // Inserts a node into the heap with the given ID and key.
    // Returns 0 on success, 1 if heap is full, 2 if ID already exists, 3 if hash table insertion fails.
    int insert(const string &id, int key, void *pv = nullptr);

    // Updates the key of the node with the specified ID and reheapifies.
    // Returns 0 on success, 1 if ID not found.
    int setKey(const string &id, int key);

    // Deletes the root node (smallest key) and optionally returns its details.
    // Returns 0 on success, 1 if heap is empty.
    int deleteMin(string *pId = nullptr, int *pKey = nullptr, void **ppData = nullptr);

    // Removes a node with the specified ID and optionally returns its details.
    // Returns 0 on success, 1 if ID not found.
    int remove(const string &id, int *pKey = nullptr, void **ppData = nullptr);

    // Stores the current number of elements in the heap.
    int currentSize;

private:
    // Represents each node in the heap.
    class node
    {
    public:
        string id;   // Unique identifier for the node.
        int key;     // Key value used to maintain heap order.
        void *pData; // Pointer to associated data.
    };

    int capacity;      // Maximum number of nodes the heap can hold.
    vector<node> data; // Array-based representation of the heap (1-based indexing).
    hashTable mapping; // Hash table for quick lookup of nodes by ID.

    // Moves the node at the specified position up the heap to restore order.
    void percolateUp(int posCur);

    // Moves the node at the specified position down the heap to restore order.
    void percolateDown(int posCur);

    // Retrieves the position of the specified node in the heap array.
    int getPos(node *pn);

    // Verifies if the heap contains the given ID.
    bool contains(const string &id) const;
};

#endif
