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
* This function returns the next token in the given file. It first mallocs 5 bytes and keeps track of the size of the token.
* It goes through the file one char at a time, and starts when an alphabetical character is reached. If the end of file is reached
* before then, this function returns NULL. Otherwise, the token expands to contain all alphanumerical values afterwards and stops
* when it reaches a non-alphanumerical value. Then the function returns that token.
*/
char* getToken(FILE file) {
	int currentSize = 0;
	int maxSize = 5;
	char* nextToken = malloc(maxSize);
	char nextChar = getc(file);
	// Iterates through the file until an alphabetical character is reached.
	while (!isalpha(nextChar) && nextChar != EOF) {
		nextChar = getc(file);
	}
	// If the end of the file is reached before another token is found, then return NULL.
	if (nextChar == EOF) {
		return NULL;
	}
	nextToken = nextChar;
	currentSize++;
	// Add all subsequent alphanumerical characters to the token.
	while (isalnum(nextChar)) {
		nextToken[currentSize] = nextChar;
		currentSize++;
		// Dynamically reallocate memory if the current size is exceeded.
		if (currentSize > maxSize) {
			nextToken = realloc(nextToken, maxSize + 5);
		}
		nextChar = getc(file);
	}
	if (currentSize > maxSize) {
		nextToken = realloc(nextToken, maxSize + 1);
		currentSize++;
	}
	nextToken[currentSize] = '/0';
	return nextToken;
}

/*
* This function takes the string representation of a file name and opens it and reads it. It opens the file in read mode 
* and iterates through one character at a time to get each string token. It adds these strings to the allStrings stringMap.
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
	//Fetch the next token in the file. 
	char* nextToken = getToken(file);
	/*
	* Keep iterating through the tokens in the current file until it's NULL. This means that there are no more tokens
	* in the file.
	*/
	while (nextToken != NULL) {
		// Allocates memory and defines a new nameOfFile string to be stored if a new file key is created.
		char* nameOfFile = malloc(strlen(fileName) + 1);
		i = 0;
		while (i < strlen(fileName)) {
			nameOfFile[i] = fileName[i];
			i++;
		}
		nameOfFile[i] = '/0';
		stringTable* ptr = allStrings;
		stringTable* prev = NULL;
		/* 
		* Keep iterating through the hashtable of allStrings until the string value of the next token is greater than or equal to
		* the string value of the token in the hashtable or if the end of the hashtable is reached.
		*/
		while (ptr != NULL) {
			while (strcmp(newToken, ptr->string) < 0) {
				ptr = ptr->next;
			}
		}
		/*
		* If ptr is NULL, that means the end of the hashtable has been reached. In that case, insert a new key at the end with the name
		* of the next token and a new file hashtable object that contains just this file with frequency 1. 
		*/
		if (ptr == NULL) {
			fileTable* newFile = malloc(sizeof(fileTable));
			newFile->fileName = nameOfFile;
			newFile->frequency = 1;
			newFile->next = NULL;
			stringTable* newString = malloc(sizeof(stringTable));
			newString->string = newToken;
			newString->files = newFile;
			newString->next = NULL;
			if (prev != NULL) {
				prev->next = newString;
			}
			/*
			* If the next token is already in the string hashtable, iterate through its file hashtable to see if this file is present there.
			* If it is, increment the frequency by one. Otherwise, create a new file key.
			*/
		} else if (strcmp(newToken, ptr->string) == 0) {
			fileTable* fileptr = ptr->files;
			fileTable* fileprev = NULL;
			while (fileptr != NULL) {
				while (strcmp(nameOfFile, fileptr->fileName) < 0) {
					fileprev = fileptr;
					fileptr = fileptr->next;
				}
			}
			/*
			* If the end of the file hashtable is reached and the file was not found, create a new file key at the end.
			*/
			if (fileptr == NULL) {
				fileTable* newFile = malloc(sizeof(nameOfFile));
				newFile->fileName = nameOfFile;
				newFile->frequency = 1;
				newFile->next = fileptr;
				if (fileprev != NULL) {
					fileprev->next = newFile;
				}
			}
			/*
			* If the file is found, then increase its frequency. Because you didn't create a new file key, you can free nameOfFile.
			*/
			else if (strcmp(nameOfFile, fileptr->fileName) == 0) {
				fileptr->frequency++;
				free(nameOfFile);
				/*
				* If the file name is greater than the file name of the current key, that means the current file name does not exist
				* in the hashtable. Create a new file key and link it to the rest of the hashtable.
				*/
			} else {
				fileTable* newFile = malloc(sizeof(fileTable));
				newFile->fileName = nameOfFile;
				newFile->frequency = 1;
				newFile->next = fileptr;
				if (fileprev != NULL) {
					fileprev->next = newFile;
				}
			}
		/*
		* If the string value of the next token is greater than the string value of the current string in the hashtable, then the next token
		* is not in the hashtable. Create a new token key and create a new file hashtable for it.
		*/
		} else {
			fileTable* newFile = malloc(sizeof(fileTable));
			newFile->fileName = nameOfFile;
			newFile->frequency = 1;
			newFile->next = NULL;
			stringTable* newString = malloc(sizeof(stringTable));
			newString->string = newToken;
			newString->files = newFile;
			newString->next = ptr;
			if (prev != NULL) {
				prev->next = newString;
			}
		}
		// After checking for the given next token, get the next one.
		nextToken = getToken(file);
	}
	//Close the file at the end of everything.
	fclose(file);
}