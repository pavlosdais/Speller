# object files
OBJ = speller.o dictionary.o

# create the executable file
speller: $(OBJ)
	gcc -o speller $(OBJ)

# create the object files needed
speller.o: speller.c
	gcc -c speller.c

dictionary.o: dictionary.c
	gcc -c dictionary.c

# remove excess object files
clear:
	rm -f $(OBJ)

# run spell checker with the default dictionary
run:
	./speller text.txt