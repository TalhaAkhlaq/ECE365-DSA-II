/* Name: Talha Akhlaq
Description: This program implements a hash table using a polynomial rolling hash function with
linear probing for collision resolution and rehashing triggered when the load factor exceeds 0.5.
*/

#include "hash.h"
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

// Precomputed prime numbers for rehashing
const unsigned int primeNumbers[] = {53, 97, 193, 389, 769, 1543, 3079, 6151, 12289, 24593, 49157, 98317,
                                     196613, 393241, 786433, 1572869, 3145739, 6291469, 12582917, 25165843,
                                     50331653, 100663319, 201326611, 402653189, 805306457, 1610612741};

// Returns a prime number greater than the provided size for resizing
unsigned int hashTable::getPrime(int size)
{
  for (unsigned int prime : primeNumbers)
  {
    if (prime > size)
    {
      return prime;
    }
  }
  return primeNumbers[sizeof(primeNumbers) / sizeof(primeNumbers[0]) - 1]; // Return the largest available prime when no larger one is found
}

// Set loadFactor to 0.5 unconditionally
hashTable::hashTable(int size) : data(getPrime(size))
{
  capacity = data.size();
  filled = 0;
  loadFactor = 0.5;
}

// Polynomial rolling hash function for strings
unsigned int hashTable::hash(const std::string &key)
{
  unsigned int hash_value = 0;
  const unsigned int prime = 37;
  for (char c : key)
  {
    hash_value = (hash_value * prime + static_cast<unsigned char>(c)) % capacity;
  }
  return hash_value;
}

// Finds the position of the key using linear probing
int hashTable::findPos(const std::string &key)
{
  int hashIndex = hash(key);
  int startIdx = hashIndex;

  while (data[hashIndex].isOccupied || data[hashIndex].isDeleted)
  {
    if (data[hashIndex].isOccupied && !data[hashIndex].isDeleted && data[hashIndex].key == key)
    {
      return hashIndex;
    }
    hashIndex = (hashIndex + 1) % capacity;

    if (hashIndex == startIdx)
    {
      break; // probed all slots
    }
  }
  return -1; // key not found
}

// Inserts a key, resizes table if load factor exceeds 0.5, unless during rehash
int hashTable::insert(const std::string &key, void *pv, bool duringRehash)
{
    if (findPos(key) != -1)
    {
        return 1; // Key already exists
    }

    // Skip rehashing if we are currently rehashing
    if (!duringRehash && filled >= capacity * loadFactor)
    {
        if (!rehash())
        {
            return 2; // Rehashing failed
        }
    }

    int hashIndex = hash(key);
    int firstDeletedIndex = -1;

    while (data[hashIndex].isOccupied)
    {
        if (data[hashIndex].isDeleted && firstDeletedIndex == -1)
        {
            firstDeletedIndex = hashIndex;
        }
        hashIndex = (hashIndex + 1) % capacity; // Linear probing
    }

    int insertIndex = (firstDeletedIndex != -1) ? firstDeletedIndex : hashIndex;

    data[insertIndex].key = key;
    data[insertIndex].isOccupied = true;
    data[insertIndex].isDeleted = false;
    data[insertIndex].pv = pv;

    if (firstDeletedIndex == -1)
    {
        filled++; // Increment filled only if inserting into a new slot
    }

    return 0;
}

// Rehashes the table and redistributes keys when load factor is exceeded
bool hashTable::rehash()
{
    int oldCapacity = capacity;
    int newCapacity = getPrime(2 * oldCapacity);

    if (newCapacity <= oldCapacity)
    {
        // Unable to find a larger prime number
        return false;
    }

    capacity = newCapacity;
    std::vector<hashItem> oldData = data;
    data = std::vector<hashItem>(capacity);
    filled = 0;

    for (const auto &item : oldData)
    {
        if (item.isOccupied && !item.isDeleted)
        {
            insert(item.key, item.pv, true); // Bypass flag to prevent rehashing
        }
    }
    return true;
}

// Checks if a key exists in the table
bool hashTable::contains(const std::string &key)
{
  return findPos(key) != -1;
}

// Returns the pointer associated with the key, if found
void *hashTable::getPointer(const std::string &key, bool *b)
{
  int pos = findPos(key);
  if (b != nullptr)
  {
    *b = (pos != -1);
  }
  return (pos != -1) ? data[pos].pv : nullptr;
}

// Updates the pointer associated with a key
int hashTable::setPointer(const std::string &key, void *pv)
{
  int pos = findPos(key);
  if (pos == -1)
  {
    return 1;
  }
  data[pos].pv = pv;
  return 0;
}

// Marks a key as deleted (lazy deletion)
bool hashTable::remove(const std::string &key)
{
  int pos = findPos(key);
  if (pos == -1)
  {
    return false;
  }
  data[pos].isDeleted = true;
  return true;
}