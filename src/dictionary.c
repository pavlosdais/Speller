#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table - hash table's size
// According to theory, the hash table's size ideally is a prime number
// these sizes have have proven to have good behaviour:
// 53, 97, 193, 389, 769, 1543, 3079, 6151, 12289, 24593, 49157, 98317, 196613, 393241 ..
#define N 1543

// Hash table
node *table[N];
int NUM_OF_WORDS;

char lowerCase(char character)
{
    if (isupper(character))
        return tolower(character);
    
    return character;
}

/* Hashes word to a number using the polynomial rolling hash function
   source: https://www.geeksforgeeks.org/string-hashing-using-polynomial-rolling-hash-function/ */
unsigned int hash(const char *word)
{
    const int p = 111111, m = 1e9 + 7;
    int hash = 0;
    long p_pow = 1;
    for(int i = 0; word[i] != '\0'; i++)
    {
        hash = (hash + lowerCase(word[i]) * p_pow) % m;
        p_pow = (p_pow * p) % m;
    }
    return hash % N;
}

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    node* linked_list = table[hash(word)];

    while (linked_list != NULL)
    {
        if (strcasecmp(word, linked_list->word) == 0)  // word found in the dictionary, return true
            return true;
        else  // not found, go to the next word
            linked_list = linked_list->next;
    }

    // search unsuccessful, return false
    return false;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE *dict;
    dict = fopen(dictionary, "r");
    if (dict == NULL) return false;  // error in trying to open the file

    char *word = malloc(sizeof(char)*LENGTH + 1);
    if (word == NULL) return false;  // allocation failure
    
    NUM_OF_WORDS = 0;
    while ((fscanf(dict, "%s", word)) != EOF)
    {
        // create node in order to store the word
        node* new_node = malloc(sizeof(node));
        if (new_node == NULL) return false;  // allocation failure

        // find the correct bucket the word needs to be inserted into
        unsigned int index = hash(word);

        // fill the node's contents
        strcpy(new_node->word, word);

        // insert the word to that bucket
        new_node->next = table[index];
        table[index] = new_node;
        NUM_OF_WORDS++;
    }

    free(word);
    return true;
}

// Returns number of words in dictionary if loaded
unsigned int size(void) { return NUM_OF_WORDS; }

// Frees dictionary from memory
void unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node* linked_list = table[i];
        while(linked_list != NULL)
        {
            node* tmp = linked_list;
            linked_list = linked_list->next;
            free(tmp);
        }
    }
}