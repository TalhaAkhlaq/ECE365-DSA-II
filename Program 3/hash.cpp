/* Name: Talha Akhlaq
   Description: Implements a hash table using polynomial rolling hash and linear probing with rehashing
   when the load factor is exceeded; supports insertion, search, pointer retrieval and update, and lazy deletion.
*/

#include "hash.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Precomputed prime numbers for resizing during rehash.
const unsigned int primeNumbers[] = {53, 97, 193, 389, 769, 1543, 3079, 6151, 12289, 24593, 49157, 98317,
                                     196613, 393241, 786433, 1572869, 3145739, 6291469, 12582917, 25165843,
                                     50331653, 100663319, 201326611, 402653189, 805306457, 1610612741};

// Finds a suitable prime number for resizing based on the current size.
unsigned int hashTable::getPrime(int size)
{
    for (unsigned int prime : primeNumbers)
    {
        if (prime > static_cast<unsigned int>(size))
        {
            return prime;
        }
    }
    // Returns the largest prime if no larger one is available.
    return primeNumbers[sizeof(primeNumbers) / sizeof(primeNumbers[0]) - 1];
}

// Initializes the hash table with a prime size close to the specified value.
hashTable::hashTable(int size) : data(getPrime(size))
{
    capacity = data.size();
    filled = 0;
    loadFactor = 0.5; // Default load factor threshold for rehashing.
}

// Computes hash value using a polynomial rolling hash.
unsigned int hashTable::hash(const string &key) const
{
    unsigned int hash_value = 0;
    const unsigned int prime = 37; // Base prime for polynomial hash calculation.
    for (char c : key)
    {
        hash_value = (hash_value * prime + static_cast<unsigned char>(c)) % capacity;
    }
    return hash_value;
}

// Finds position of the specified key using linear probing.
int hashTable::findPos(const string &key) const
{
    int hashIndex = hash(key);
    int startIdx = hashIndex;

    // Probes slots until key is found or loop returns to start index.
    while (data[hashIndex].isOccupied || data[hashIndex].isDeleted)
    {
        if (data[hashIndex].isOccupied && !data[hashIndex].isDeleted && data[hashIndex].key == key)
        {
            return hashIndex;
        }
        hashIndex = (hashIndex + 1) % capacity;

        if (hashIndex == startIdx) // Full table scan, key not found.
        {
            break; // Entire table has been probed.
        }
    }
    return -1; // Key not found.
}

// Inserts key into the table, rehashing if load factor is exceeded.
int hashTable::insert(const string &key, void *pv, bool duringRehash)
{
    if (findPos(key) != -1)
    {
        return 1; // Key already exists.
    }

    // Checks load factor and rehashes if necessary (unless already rehashing).
    if (!duringRehash && filled >= static_cast<int>(capacity * loadFactor))
    {
        if (!rehash())
        {
            return 2; // Rehashing failed.
        }
    }

    int hashIndex = hash(key);
    int firstDeletedIndex = -1;

    // Probes for an available slot, saving the first deleted slot if found.
    while (data[hashIndex].isOccupied)
    {
        if (data[hashIndex].isDeleted && firstDeletedIndex == -1)
        {
            firstDeletedIndex = hashIndex;
        }
        hashIndex = (hashIndex + 1) % capacity;

        if (hashIndex == hash(key))
        {
            break; // All slots have been probed; should not occur if rehashing works (avoid infinite loop).
        }
    }

    // Uses the first deleted slot if available; otherwise, uses the current hash index.
    int insertIndex = (firstDeletedIndex != -1) ? firstDeletedIndex : hashIndex;

    data[insertIndex].key = key;
    data[insertIndex].isOccupied = true;
    data[insertIndex].isDeleted = false;
    data[insertIndex].pv = pv;

    if (firstDeletedIndex == -1)
    {
        filled++; // Only increments if slot was not previously deleted.
    }

    return 0; // Insertion successful.
}

// Rehashes the table by doubling its size to the next prime number and redistributing keys.
bool hashTable::rehash()
{
    int oldCapacity = capacity;
    int newCapacity = getPrime(2 * oldCapacity);

    if (newCapacity <= oldCapacity)
    {
        return false; // No larger prime available for resizing.
    }

    capacity = newCapacity;
    vector<hashItem> oldData = data;
    data = vector<hashItem>(capacity);
    filled = 0;

    // Reinserts all active keys into the resized table.
    for (const auto &item : oldData)
    {
        if (item.isOccupied && !item.isDeleted)
        {
            insert(item.key, item.pv, true); // Bypasses rehash trigger during rehash.
        }
    }
    return true; // Rehash successful.
}

// Checks if the key exists in the table.
bool hashTable::contains(const string &key) const
{
    return findPos(key) != -1;
}

// Retrieves pointer associated with key; sets `b` to indicate presence.
void *hashTable::getPointer(const string &key, bool *b) const
{
    int pos = findPos(key);
    if (b != nullptr)
    {
        *b = (pos != -1);
    }
    return (pos != -1) ? data[pos].pv : nullptr;
}

// Updates the pointer associated with an existing key.
int hashTable::setPointer(const string &key, void *pv)
{
    int pos = findPos(key);
    if (pos == -1)
    {
        return 1; // Key not found.
    }
    data[pos].pv = pv;
    return 0; // Update successful.
}

// Marks the key as deleted for future reuse of slot (lazy deletion).
bool hashTable::remove(const string &key)
{
    int pos = findPos(key);
    if (pos == -1)
    {
        return false; // Key not found.
    }
    data[pos].isDeleted = true;
    return true; // Successfully marked as deleted.
}
