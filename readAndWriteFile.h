#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <fcntl.h>

/*
* This is a hashtable to store the name of a file and the frequency of a given string in this file. It will be stored
* in a hashtable of strings to represent the frequency of that string in each file. It contains a pointer to the next 
* file in the list.
*/
typedef struct _fileTable {
	char* fileName;
	int frequency;
	struct _fileTable* next;
	struct _fileTable* prev;
} fileTable;

/*
* This is a struct to create a hashtable of strings. Each stringMap contains a string, a pointer to the next string in
* the hashtable, and a pointer to a hashtable of files which holds the string's frequency in each file in the hashtable.
*/
typedef struct _stringTable {
	char* string;
	fileTable* files;
	struct _stringTable* next;
} stringTable;

char* getToken(int file);
int readFile(char* fileName);
int writeFile(char* fileName, stringTable* hashtable);