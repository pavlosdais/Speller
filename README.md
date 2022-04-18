# Speller
> Harvard's CS50 Introduction to Programming [Week 5](https://cs50.harvard.edu/college/2022/spring/psets/5/speller/#speller) (Data Structures) project. It's important to note, though, that there are changes to the original format. <br/>

* **Implementation** <br/>
The program takes as input a dictionary and a text file. The dictionary is loaded into a hash table then the words with alphabetical characters in text file are searched to check if they exist in the hash table. For the sake of simplicity, the speller is case-insensitive and no word is longer than 45 characters. In other words, if foo is in dictionary, then check should return true given any capitalization thereof; none of foo, foO, fOo, fOO, fOO, Foo, FoO, FOo, and FOO should be considered misspelled. The dictionary provided (large_dictionary.txt) uses 143,091 words and no word appears more than once. But, you are able to use any dictionary passed to the program structured alphabetically sorted from top to bottom with one word per line, each of which ends with \n (new line). The output of the program is the actual words that were misspelled, the number of words in the dictionary, the number of words misspelled as well as some benchmarks about the time spent loading the dictionary and the search time.

* **Hashing** <br/>
The hash function used is deterministic, meaning no matter how many times you pass the same word, the same output or key(a positive integer) is generated which is used to map the word to it. The way the mapping works is by using the djb2 hash function (source [here](https://theartincode.stanis.me/008-djb2/)).

* **Usage** <br/>
* > Compile program: make <br/>
* > Run program: ./speller [DICTIONARY] text *OR* make run (update the text.txt file) <br/>
  for example: ./speller large_dictionary.txt text.txt <br/>
