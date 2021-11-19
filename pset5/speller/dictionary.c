// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <math.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table - choosing an efficient prime number
// Ref: https://www.geeksforgeeks.org/string-hashing-using-polynomial-rolling-hash-function/
// Ref: https://sunilsavanur.wordpress.com/2012/08/14/polynomial-hash-function-for-dictionary-words/
const unsigned int N = 1000003;

// Init global var for tallying dictionary size during load
int dictionary_size = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Get hash key
    int key = hash(word);

    // Access linked list at that key
    node *head = table[key];

    // Traverse list looking for word
    for (node *cursor = head; cursor != NULL; cursor = cursor->next)
    {
        if (strcasecmp(word, cursor->word) == 0)
        {
            return true;
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // Polynomial rolling hash function
    // Ref: https://www.geeksforgeeks.org/string-hashing-using-polynomial-rolling-hash-function/
    // Ref: https://sunilsavanur.wordpress.com/2012/08/14/polynomial-hash-function-for-dictionary-words/

    // Choose a prime number roughly equal to the number of
    // different characters used (here we're using 26 letters + apostrophe)
    int p = 31;

    long long result = 0;
    long long power_of_p = 1;

    for (int i = 0, n = strlen(word); i < n; i++)
    {
        result = (result + toupper(word[i]) * power_of_p) % N;
        power_of_p = (power_of_p * p) % N;
    }

    // Return positive result (within bounds of table)
    return (result % N + N) % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        printf("Error while opening dictionary file\n");
        return false;
    }

    // Buffer to hold current word
    char word[LENGTH + 1];

    // Get each word from the file into the buffer
    while (fscanf(file, "%s", word) != EOF)
    {
        // Create a new node for the word
        node *new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            printf("Error allocating memory for dictionary word\n");
            return false;
        }

        // Copy word from buffer into node
        strcpy(new_node->word, word);

        // Get hash for this word and insert node into hash table
        int key = hash(word);
        new_node->next = table[key];
        table[key] = new_node;
        dictionary_size++;
    }

    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return dictionary_size;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // Create a second pointer so we can move the pointer at the
    // head of the linked list and also know what node to free
    node *tmp = NULL;

    // For each index in hash table
    for (int i = 0; i < N; i++)
    {
        node *head = table[i];
        while (head != NULL)
        {
            tmp = head;
            head = head-> next;
            free(tmp);
        }
    }

    return true;
}
