// Implements a dictionary's functionality
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

// Number of buckets in hash table
#define N 1000

// Hash table
node *table[N];
int NUM_OF_WORDS;

char lowerCase(char character)
{
    if (isupper(character))
        return tolower(character);
    
    return character;
}

/* Hashes word to a number using djb2 hash function
   source: https://theartincode.stanis.me/008-djb2/ */
unsigned int hash(const char *word)
{
    unsigned int hash = 5381, i = 0;
    while(word[i] != '\0')
    {
        hash = ((hash) << 5 + hash) + lowerCase(word[i]);  // hash * 33 + lowerCase(word[i])
        i++;
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
