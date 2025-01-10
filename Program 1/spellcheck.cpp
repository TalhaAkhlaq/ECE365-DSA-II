/* Name: Talha Akhlaq
Description: This program implements a spell-checker using a hash table to load a dictionary
and checks an input document for invalid words, reporting results to an output file.
*/

#include "hash.h"
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <algorithm>
#include <cctype>

using namespace std;

// Load the dictionary into the hash table
hashTable loadDictionary(const string &dictionaryFile)
{
  ifstream dictStream(dictionaryFile);

  if (!dictStream.is_open())
  {
    cerr << "Error: Could not open dictionary file: " << dictionaryFile << endl;
    exit(EXIT_FAILURE);
  }

  // Initialize the hash table
  hashTable dictionary(100000);

  string word;
  while (getline(dictStream, word))
  {
    // Early length check to skip words that are too long
    if (word.length() > 20)
    {
      continue;
    }

    bool isValid = true;

    // Process each character in the word
    for (size_t i = 0; i < word.length(); ++i)
    {
      char &c = word[i];

      // Convert to lowercase if it's an uppercase letter (A-Z)
      if (c >= 'A' && c <= 'Z')
      {
        c += 'a' - 'A';
      }

      // Check for invalid characters: anything other than lowercase letters, digits, dash, or apostrophe
      if (!((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '-' || c == '\''))
      {
        isValid = false; // Invalid character found
        break;           // No need to check further characters, break early
      }
    }

    // Insert valid words into the hash table
    if (isValid)
    {
      int result = dictionary.insert(word);
      if (result == 2)
      {
        cerr << "Error: Rehashing failed during dictionary loading." << endl;
        exit(EXIT_FAILURE);
      }
    }
  }

  dictStream.close();
  return dictionary;
}

// Spell-check the input file and write results to the output file
void spellCheck(const string &inputFile, const string &outputFile, hashTable &dictionary)
{
  ifstream inputStream(inputFile);
  ofstream outputStream(outputFile);

  if (!inputStream.is_open())
  {
    cerr << "Error: Could not open input file: " << inputFile << endl;
    exit(EXIT_FAILURE);
  }
  if (!outputStream.is_open())
  {
    cerr << "Error: Could not open output file: " << outputFile << endl;
    exit(EXIT_FAILURE);
  }

  string line;
  int lineNumber = 1;

  // Process each line in the input file
  while (getline(inputStream, line))
  {
    size_t i = 0;
    size_t len = line.length();

    while (i < len)
    {
      // Skip non-valid characters (word separators)
      while (i < len && !(
                            (line[i] >= 'A' && line[i] <= 'Z') ||
                            (line[i] >= 'a' && line[i] <= 'z') ||
                            (line[i] >= '0' && line[i] <= '9') ||
                            line[i] == '-' || line[i] == '\''))
      {
        i++;
      }

      // Start of a word
      if (i >= len)
        break;

      size_t wordStart = i;
      size_t wordLength = 0;
      bool hasDigit = false;
      char wordBuffer[21]; // 20 characters max + null terminator

      // Collect valid word characters
      while (i < len && ((line[i] >= 'A' && line[i] <= 'Z') ||
                         (line[i] >= 'a' && line[i] <= 'z') ||
                         (line[i] >= '0' && line[i] <= '9') ||
                         line[i] == '-' || line[i] == '\''))
      {
        char c = line[i];

        // Check for digits
        if (c >= '0' && c <= '9')
        {
          hasDigit = true;
        }

        // Convert uppercase letters to lowercase
        if (c >= 'A' && c <= 'Z')
        {
          c = c + ('a' - 'A'); // Convert to lowercase
        }

        // Add character to word buffer if within length limit
        if (wordLength < 20)
        {
          wordBuffer[wordLength++] = c;
        }

        i++;
      }

      size_t totalWordLength = i - wordStart;

      // Process the word
      if (totalWordLength > 20)
      {
        outputStream << "Long word at line " << lineNumber << ", starts: ";
        outputStream.write(wordBuffer, 20);
        outputStream << endl;
      }
      else if (!hasDigit)
      {
        wordBuffer[wordLength] = '\0'; // Null-terminate the word

        if (!dictionary.contains(string(wordBuffer)))
        {
          outputStream << "Unknown word at line " << lineNumber << ": " << wordBuffer << endl;
        }
      }
    }

    lineNumber++;
  }

  inputStream.close();
  outputStream.close();
}

int main()
{
  string dictFile, inputFile, outputFile;

  // Prompt user for input/output files and dictionary
  cout << "Enter name of dictionary: ";
  cin >> dictFile;
  cout << "Enter name of input file: ";
  cin >> inputFile;
  cout << "Enter name of output file: ";
  cin >> outputFile;

  // Measure time to load dictionary
  clock_t startTime = clock();
  hashTable dictionary = loadDictionary(dictFile);
  clock_t endTime = clock();
  double dictLoadTime = double(endTime - startTime) / CLOCKS_PER_SEC;
  cout << "Total time (in seconds) to load dictionary: " << dictLoadTime << endl;

  // Measure time to check the document
  startTime = clock();
  spellCheck(inputFile, outputFile, dictionary);
  endTime = clock();
  double spellCheckTime = double(endTime - startTime) / CLOCKS_PER_SEC;
  cout << "Total time (in seconds) to check document: " << spellCheckTime << endl;

  return 0;
}
