#ifndef _HASH_H
#define _HASH_H

#include <vector>
#include <string>

using namespace std;

class hashTable
{
public:
    // Initializes the hash table with a prime size based on the specified value.
    hashTable(int size = 0);

    // Inserts a key into the hash table with an optional pointer, returning 0 on success, 1 if the key exists, or 2 if rehashing fails.
    int insert(const string &key, void *pv = nullptr, bool duringRehash = false);

    // Checks if a key exists in the table, returning true if found, false otherwise.
    bool contains(const string &key) const;

    // Retrieves pointer associated with the key; sets `b` to true if key exists, false otherwise.
    void *getPointer(const string &key, bool *b = nullptr) const;

    // Updates the pointer associated with a key, returning 0 on success or 1 if the key is not found.
    int setPointer(const string &key, void *pv);

    // Marks a key as deleted (lazy deletion), returning true on success or false if the key is not found.
    bool remove(const string &key);

private:
    // Represents an individual entry in the hash table.
    class hashItem
    {
    public:
        string key{""};         // Stores the key for this item.
        bool isOccupied{false}; // Indicates if this slot is occupied.
        bool isDeleted{false};  // Indicates if this slot has been lazily deleted.
        void *pv{nullptr};      // Pointer associated with the key, if provided.

        hashItem() = default; // Default constructor for initializing a hash item.
    };

    int capacity;      // Current capacity of the table.
    int filled;        // Count of occupied (non-deleted) items.
    double loadFactor; // Threshold load factor to trigger rehash.

    vector<hashItem> data; // Storage for hash items.

    // Computes a hash value for a string using a polynomial rolling hash function.
    unsigned int hash(const string &key) const;

    // Finds the position of a key in the table using linear probing, returning the index or -1 if not found.
    int findPos(const string &key) const;

    // Resizes the hash table when the load factor exceeds the threshold, returning true if successful.
    bool rehash();

    // Finds a prime number larger than the given size for resizing.
    static unsigned int getPrime(int size);
};

#endif //_HASH_H
