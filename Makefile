PROGRAMS = invertedIndex
CC = gcc

invertedIndexMake: readAndWriteFile.h dirFunctions.h
	$(CC) invertedIndex.c -o invertedIndex
	 
.PHONY: all clean

all: $(PROGRAMS)

clean:
	@rm -f *.o $(PROGRAMS)