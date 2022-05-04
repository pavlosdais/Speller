#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "dictionary.h"
#include <sys/resource.h>
#include <sys/time.h>

// Undefine any definitions
#undef calculate
#undef getrusage

// Function Prototype
double calculate(const struct rusage *b, const struct rusage *a);

int main(int argc, char *argv[])
{
    // Check for correct number of args
    if (argc != 2 && argc != 3)
    {
        printf("Usage: %s [DICTIONARY] text\n", argv[0]);
        return 1;
    }

    // Determine what dictionary to use - default:large_dictionary.txt
    char *dictionary = (argc == 3) ? argv[1] : DICTIONARY;

    // Structures for timing data
    struct rusage before, after;

    // Benchmarks
    double time_load = 0.0, time_check = 0.0;

    // Load dictionary
    getrusage(RUSAGE_SELF, &before);
    bool loaded = load(dictionary);
    getrusage(RUSAGE_SELF, &after);

    // Exit if dictionary was not loaded
    if (!loaded)
    {
        printf("Error! Could not load %s.\n", dictionary);
        return 1;
    }

    // Calculate time to load the dictionary
    time_load = calculate(&before, &after);

    // Try to open the text (read mode)
    char *text = (argc == 3) ? argv[2] : argv[1];
    FILE *file = fopen(text, "r");
    if (file == NULL)
    {
        printf("Could not open %s, exiting..\n", text);
        unload();
        return 1;
    }

    // Prepare to spell-check
    int index = 0, misspellings = 0, words = 0;

    char* word = malloc(sizeof(char)*LENGTH + 1);
    if (word == NULL)  
    {
        printf("Allocation failure, exiting..\n");
        unload();
        fclose(file);
        return 1;
    }
    
    // Spell-check each word in text
    printf("\nMISSPELLED WORDS:\n");
    char c;
    while (fread(&c, sizeof(char), 1, file))
    {
        // Only allow alphabetical characters and apostrophes
        if (isalpha(c) || (c == '\'' && index > 0))
        {
            // Append character to word
            word[index] = c;
            index++;

            // Ignore alphabetical strings too long to be words
            if (index > LENGTH)
            {
                // Consume remainder of alphabetical string
                while (fread(&c, sizeof(char), 1, file) && isalpha(c));

                // Prepare for new word
                index = 0;
            }
        }

        // Ignore words with numbers (like MS Word can)
        else if (isdigit(c))
        {
            // Consume remainder of alphanumeric string
            while (fread(&c, sizeof(char), 1, file) && isalnum(c));

            // Prepare for new word
            index = 0;
        }

        // We must have found a whole word
        else if (index > 0)
        {
            // Terminate current word
            word[index] = '\0';

            // Update counter
            words++;

            // Check word's spelling
            getrusage(RUSAGE_SELF, &before);
            bool misspelled = check(word);
            getrusage(RUSAGE_SELF, &after);

            // Update benchmark
            time_check += calculate(&before, &after);

            // Print word if it is misspelled
            if (!misspelled)
            {
                printf("%s\n", word);
                misspellings++;
            }

            // Prepare for next word
            index = 0;
        }
    }
    free(word);

    // Check whether there was an error
    if (ferror(file))
    {
        printf("Error reading %s.\n", text);
        return 1;
    }

    // Close text
    fclose(file);

    // Unload dictionary
    unload();

    // Report benchmarks
    printf("\nWords misspelled:          %d\n", misspellings);
    printf("Words in dictionary:       %d\n", size());
    printf("Words in text:             %d\n", words);
    printf("Time loading dictionary:   %.2f\n", time_load);
    printf("Time spellchecking:        %.2f\n", time_check);
    printf("Total time:                %.2f\n\n", time_load + time_check);

    return 0;
}

// Returns number of seconds between b and a
double calculate(const struct rusage *b, const struct rusage *a)
{
    if (b == NULL || a == NULL) return 0.0;
    else
    {
        return ((((a->ru_utime.tv_sec * 1000000 + a->ru_utime.tv_usec) -
                  (b->ru_utime.tv_sec * 1000000 + b->ru_utime.tv_usec)) +
                 ((a->ru_stime.tv_sec * 1000000 + a->ru_stime.tv_usec) -
                  (b->ru_stime.tv_sec * 1000000 + b->ru_stime.tv_usec)))
                / 1000000.0);
    }
}
