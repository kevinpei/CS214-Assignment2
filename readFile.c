#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

/*
* This is a hashtable to store the name of a file and the frequency of a given string in this file. It will be stored
* in a hashtable of strings to represent the frequency of that string in each file. It contains a pointer to the next 
* file in the list.
*/
typedef struct _fileTable {
	char* fileName;
	int frequency;
	struct _fileTable* next;
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

static stringTable* allStrings;

char* checkNextChar(char* string, char nextChar) {
	if (!isalnum(nextChar))
}

/*
* This function takes the string representation of a file name and opens it and reads it. It opens the file in read mode 
and iterates through one character at a time to get each string token. It adds these strings to the allStrings stringMap.
*/
void readFile(char* fileName) {
	i = 0;
	/*
	* This converts the file name to all lower case.
	*/
	while (i < strlen(fileName)) {
		fileName[i] = tolower(fileName[i]);
		i++;
	}
	FILE file = fopen(fileName, 'r');
	/*
	* If the file does not exist, throw an error and return.
	*/
	if (file == NULL) {
		printf("Specified file does not exist\n");
		return;
	}
	char nextChar = getc(file);
	while (nextChar != EOF) {
		if (!isalnum(nextChar)) {
			
		}
		char* currentWord = 
		nextChar = getc(file);
	}
}