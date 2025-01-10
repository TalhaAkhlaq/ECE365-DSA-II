/* Name: Talha Akhlaq
   Description:
   This program determines whether a given string C is a valid merge of two strings A and B.
   A valid merge contains all characters from A and B in a sequence that preserves the original
   order of characters from both strings. If C is a valid merge, the program outputs C with
   characters originating from A converted to uppercase, ensuring that these characters appear
   as early as possible within C. If C is not a valid merge of A and B, the program outputs
   "*** NOT A MERGE ***".
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>    // For toupper
#include <algorithm> // For reverse
#include <cstring>   // For memset

using namespace std;

// Maximum sizes based on constraints
const int MAX_A = 1000;
const int MAX_B = 1000;

// Global memoization table initialized to -1 (uncomputed states)
// memo_table[i][j] = 1 indicates that C[i+j...] can be formed by merging A[i...] and B[j...]
// memo_table[i][j] = 0 indicates that C[i+j...] cannot be formed by merging A[i...] and B[j...]
int memo_table[MAX_A + 1][MAX_B + 1];

// Global choice table to track the origin of each character in the merged string ('A' or 'B')
// choice_table[i][j] = 'A' means the character at C[i+j] is taken from A[i]
// choice_table[i][j] = 'B' means the character at C[i+j] is taken from B[j]
char choice_table[MAX_A + 1][MAX_B + 1];

/**
 * Function: recurse
 * -----------------
 * Recursively checks if string C can be formed by merging strings A and B starting from indices i and j
 * Records choices to reconstruct the merged path later
 *
 * @param i     Current index in string A
 * @param j     Current index in string B
 * @param A     The first input string
 * @param B     The second input string
 * @param C     The target merged string to be validated
 *
 * @return      1 if a valid merge path exists from the current indices, else 0
 */
int recurse(int i, int j, const string &A, const string &B, const string &C)
{
  // If already computed, return the stored result
  if (memo_table[i][j] != -1)
  {
    return memo_table[i][j];
  }

  // If both A and B have been fully traversed, and so has C, a valid merge is found
  if (i == A.size() && j == B.size())
  {
    memo_table[i][j] = 1;
    return 1;
  }

  // If the current position exceeds the length of C, no valid merge is possible
  if (i + j >= C.size())
  {
    memo_table[i][j] = 0;
    return 0;
  }

  // Attempt to take the next character from A if it matches the corresponding character in C
  if (i < A.size() && A[i] == C[i + j])
  {
    if (recurse(i + 1, j, A, B, C))
    {
      choice_table[i][j] = 'A'; // Record the choice to take from A
      memo_table[i][j] = 1;     // Mark this state as valid
      return 1;
    }
  }

  // Attempt to take the next character from B if it matches the corresponding character in C
  if (j < B.size() && B[j] == C[i + j])
  {
    if (recurse(i, j + 1, A, B, C))
    {
      choice_table[i][j] = 'B'; // Record the choice to take from B
      memo_table[i][j] = 1;     // Mark this state as valid
      return 1;
    }
  }

  // If neither option is valid, mark this state as invalid
  memo_table[i][j] = 0;
  return 0;
}

/**
 * Function: isValidMerge
 * ----------------------
 * Determines whether string C is a valid merge of strings A and B
 * If valid, reconstructs the merged string with characters from A in uppercase
 *
 * @param A       The first input string
 * @param B       The second input string
 * @param C       The target merged string to be validated
 * @param merged  Reference to a string where the resulting merged string will be stored if valid
 *
 * @return        True if C is a valid merge of A and B, else False
 */
bool isValidMerge(const string &A, const string &B, const string &C, string &merged)
{
  // Preliminary length check to quickly identify invalid merges
  if (A.size() + B.size() != C.size())
  {
    return false;
  }

  // Initialize memoization and choice tables
  memset(memo_table, -1, sizeof(memo_table));
  memset(choice_table, 0, sizeof(choice_table));

  // Start the recursive validation from the beginning of both strings
  if (recurse(0, 0, A, B, C))
  {
    // Reconstruct the merged string based on recorded choices
    int i = 0, j = 0;
    merged = "";
    while (i < A.size() || j < B.size())
    {
      if (i < A.size() && choice_table[i][j] == 'A')
      {
        merged += static_cast<char>(toupper(A[i])); // Convert A's character to uppercase
        i++;
      }
      else if (j < B.size() && choice_table[i][j] == 'B')
      {
        merged += B[j]; // Keep B's character in lowercase
        j++;
      }
      else
      {
        // This condition should not occur if recurse returned true
        break;
      }
    }
    return true;
  }
  else
  {
    return false; // No valid merge exists
  }
}

int main()
{
  string inputFileName, outputFileName;

  // Prompt the user to enter the input and output file names
  cout << "Enter name of input file: ";
  cin >> inputFileName;
  cout << "Enter name of output file: ";
  cin >> outputFileName;

  // Open the specified input and output files
  ifstream inputFile(inputFileName);
  ofstream outputFile(outputFileName);

  if (!inputFile.is_open())
  {
    cerr << "Error: Could not open input file '" << inputFileName << "'." << endl;
    return 1;
  }

  if (!outputFile.is_open())
  {
    cerr << "Error: Could not open output file '" << outputFileName << "'." << endl;
    return 1;
  }

  string A, B, C;
  string merged;

  while (getline(inputFile, A) && getline(inputFile, B) && getline(inputFile, C))
  {
    // Check if C is a valid merge of A and B.
    if (isValidMerge(A, B, C, merged))
    {
      // If valid, write the merged string with A's characters in uppercase to the output file.
      outputFile << merged << "\n";
    }
    else
    {
      // If not valid, write the error message to the output file
      outputFile << "*** NOT A MERGE ***\n";
    }
  }

  inputFile.close();
  outputFile.close();

  return 0;
}
