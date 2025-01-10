/* Name: Talha Akhlaq
   Description: Implements a binary heap with array-based structure and hash table for ID lookup,
   supporting insertions, key updates, deletions, and retrieval of the minimum element
   with percolation methods to maintain heap order.
*/

#include "heap.h"
#include <climits>
#include <iostream>

using namespace std;

// Destructor: No dynamic memory to clean up.
heap::~heap() {}

// Constructor sets up the heap with specified capacity and initializes the hash table.
heap::heap(int capacity)
{
    this->capacity = capacity;         // Maximum capacity for the heap.
    currentSize = 0;                   // Start with an empty heap.
    data.resize(capacity + 1);         // Allocate storage for nodes with 1-based indexing.
    mapping = hashTable(capacity * 2); // Hash table size set to twice the heap capacity.
}

// Inserts a node with given ID and key; returns error code on failure.
int heap::insert(const string &id, int key, void *pv)
{
    if (currentSize >= capacity)
        return 1; // Heap is full.
    if (mapping.contains(id))
        return 2; // ID already exists.

    currentSize++;
    data[currentSize].id = id;
    data[currentSize].key = key;
    data[currentSize].pData = pv;

    if (mapping.insert(id, &data[currentSize]) != 0)
        return 3; // Hash table insertion failed.

    percolateUp(currentSize); // Adjust heap to maintain order.
    return 0;                 // Insertion successful.
}

// Updates key of the node with specified ID and reorders heap as needed.
int heap::setKey(const string &id, int key)
{
    if (!mapping.contains(id))
        return 1; // ID not found.

    node *pn = static_cast<node *>(mapping.getPointer(id));
    if (!pn)
        return 1; // Pointer retrieval failed.

    int oldKey = pn->key; // Store the old key for comparison.
    pn->key = key;        // Update node's key.
    int pos = getPos(pn); // Find position of the node in the heap.

    (key > oldKey) ? percolateDown(pos) : percolateUp(pos); // Adjust heap based on key change.
    return 0;                                               // Key update successful.
}

// Removes root node (smallest key) and returns its details.
int heap::deleteMin(string *pId, int *pKey, void **ppData)
{
    if (currentSize == 0)
        return 1; // Heap is empty.

    // If provided, store the details of the root node.
    if (pId)
        *pId = data[1].id;
    if (pKey)
        *pKey = data[1].key;
    if (ppData)
        *ppData = data[1].pData;

    mapping.remove(data[1].id); // Remove root ID from hash table.

    if (currentSize > 1)
    {
        data[1] = data[currentSize];              // Replace root with last element.
        mapping.setPointer(data[1].id, &data[1]); // Update hash table.
    }
    currentSize--;

    percolateDown(1); // Restore heap property.
    return 0;         // Root deletion successful.
}

// Removes node by ID, returns details if provided.
int heap::remove(const string &id, int *pKey, void **ppData)
{
    if (!mapping.contains(id))
        return 1; // ID not found.

    node *pn = static_cast<node *>(mapping.getPointer(id)); // Retrieve pointer to the node.
    if (!pn)
        return 1; // Pointer retrieval failed.

    int pos = getPos(pn);

    // Store the details of the node if pointers are provided.
    if (pKey)
        *pKey = pn->key;
    if (ppData)
        *ppData = pn->pData;

    setKey(id, INT_MIN); // Move node to the root by setting its key to minimum value.
    deleteMin();         // Remove the root node.
    return 0;            // Node successfully removed.
}

// Moves the node at the specified position up the heap to restore order.
void heap::percolateUp(int posCur)
{
    node tmp = data[posCur];

    while (posCur > 1 && tmp.key < data[posCur / 2].key) // Continue while node's key is less than parent's key.
    {
        data[posCur] = data[posCur / 2];                    // Move parent down to the current position.
        mapping.setPointer(data[posCur].id, &data[posCur]); // Update hash table for the parent.
        posCur /= 2;                                        // Move to the parent's position.
    }

    data[posCur] = tmp;                        // Place the node in its correct position.
    mapping.setPointer(tmp.id, &data[posCur]); // Update hash table for the node.
}

// Moves the node at the specified position down the heap to restore order.
void heap::percolateDown(int posCur)
{
    node tmp = data[posCur]; // Store the node being moved.

    while (posCur * 2 <= currentSize) // Continue while there are children.
    {
        int child = posCur * 2; // Assume left child.

        // Use right child if it exists and is smaller than the left child.
        if (child != currentSize && data[child + 1].key < data[child].key)
            child++;

        if (tmp.key <= data[child].key)
            break; // Stop if the node is smaller than both children.

        data[posCur] = data[child];                         // Move child up to the current position.
        mapping.setPointer(data[posCur].id, &data[posCur]); // Update hash table for the child.
        posCur = child;                                     // Move to the child's position.
    }

    data[posCur] = tmp;                        // Place the node in its correct position.
    mapping.setPointer(tmp.id, &data[posCur]); // Update hash table for the node.
}

// Calculates the position of the node in the heap array.
int heap::getPos(node *pn)
{
    return pn - &data[0]; // Offset from the start of the array.
}