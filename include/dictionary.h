// Declares a dictionary's functionality
#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdbool.h>

// Maximum length for a word
// (e.g., pneumonoultramicroscopicsilicovolcanoconiosis)
#define LENGTH 45

// Default dictionary
#define DICTIONARY "./src/large_dictionary.txt"

// Function Prototypes
bool check(const char *word);
unsigned int hash(const char *word);
bool load(const char *dictionary);
unsigned int size(void);
void unload(void);

#endif // DICTIONARY_H
