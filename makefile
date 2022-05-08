SRC_DIR = ./src

OBJ = $(SRC_DIR)/speller.o \
	  $(SRC_DIR)/dictionary.o

EXEC = speller
CC = gcc

# create the executable file
speller: $(OBJ)
	$(CC) -o $(EXEC) $(OBJ)

# create the object files needed
speller.o: $(SRC_DIR)/speller.c
	$(CC) -c $(SRC_DIR)/speller.c

dictionary.o: $(SRC_DIR)/dictionary.c
	$(CC) -c $(SRC_DIR)/dictionary.c

# delete excess object files
clear:
	rm -f $(OBJ)

# run spell checker with the default dictionary
run:
	./speller text.txt