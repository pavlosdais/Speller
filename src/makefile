# create the executable file
speller: speller.o dictionary.o
	gcc -o speller speller.o dictionary.o

# create the object files needed
speller.o: speller.c
	gcc -c speller.c

dictionary.o: dictionary.c
	gcc -c dictionary.c

# remove excess object files
clear:
	rm -f *.o

# run spell checker
run:
	./speller large_dictionary.txt text.txt