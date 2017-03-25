#include "readFile.h"

static stringTable* allStrings;

/*
* This function returns the next token in the given file. It first mallocs 5 bytes and keeps track of the size of the token.
* It goes through the file one char at a time, and starts when an alphabetical character is reached. If the end of file is reached
* before then, this function returns NULL. Otherwise, the token expands to contain all alphanumerical values afterwards and stops
* when it reaches a non-alphanumerical value. Then the function returns that token.
*/
char* getToken(int file) {
	int currentSize = 0;
	int maxSize = 5;
	char* nextToken = malloc(maxSize);
	char* nextChar = malloc(1);
	// Iterates through the file until an alphabetical character is reached.
	while (!isalpha(*nextChar)) {
		// If the end of the file is reached before another token is found, then return NULL.
		if (read(file, nextChar, 1) == 0) {
			printf("End of file\n");
			return NULL;
		}
		printf("Searching\n");
	}
	// Add all subsequent alphanumerical characters to the token.
	while (isalnum(*nextChar)) {
		nextToken[currentSize] = tolower(*nextChar);
		currentSize++;
		printf("Reading through characters\n");
		// Dynamically reallocate memory if the current size is exceeded.
		if (currentSize > maxSize) {
			nextToken = realloc(nextToken, maxSize + 5);
		}
		printf("Chaning nextChar from %c\n", *nextChar);
		read(file, nextChar, 1);
	}
	if (currentSize > maxSize) {
		nextToken = realloc(nextToken, maxSize + 1);
		currentSize++;
	}
	nextToken[currentSize] = '\0';
	return nextToken;
}

/*
* This function takes the string representation of a file name and opens it and reads it. It opens the file in read mode 
* and iterates through one character at a time to get each string token. It adds these strings to the allStrings stringMap.
*/
void readFile(char* fileName) {
	int i = 0;
	/*
	* This converts the file name to all lower case.
	*/
	char* nameOfFile = malloc(strlen(fileName) + 1);
	while (i < strlen(fileName)) {
		printf("Changing character %d\n", i);
		nameOfFile[i] = tolower(fileName[i]);
		i++;
	}
	nameOfFile[i] = '\0';
	printf("Opening %s\n", fileName);
	int file = open(fileName, O_RDONLY);
	printf("%s opened\n", fileName);
	/*
	* If the file does not exist, throw an error and return.
	*/
	if (file == -1) {
		printf("Specified file does not exist\n");
		return;
	}
	//Fetch the next token in the file. 
	char* nextToken = getToken(file);
	printf("Tokenizer worked: Token is %s\n", nextToken);
	/*
	* Keep iterating through the tokens in the current file until it's NULL. This means that there are no more tokens
	* in the file.
	*/
	while (nextToken != NULL) {
		printf("Next token is %s\n", nextToken);
		// Allocates memory and defines a new nameOfFile string to be stored if a new file key is created.
		nameOfFile = malloc(strlen(fileName) + 1);
		i = 0;
		while (i < strlen(fileName)) {
			nameOfFile[i] = fileName[i];
			i++;
		}
		printf("Just checking\n");
		nameOfFile[i] = '\0';
		stringTable* ptr = allStrings;
		stringTable* prev = NULL;
		/* 
		* Keep iterating through the hashtable of allStrings until the string value of the next token is greater than or equal to
		* the string value of the token in the hashtable or if the end of the hashtable is reached.
		*/
		int comparisonResult;
		if (ptr == NULL) {
			comparisonResult = 0;
		} else {
			comparisonResult = strcmp(nextToken, ptr->string);
		}
		while (comparisonResult > 0) {
			printf("iterate\n");
			prev = ptr;
			ptr = ptr->next;
			if (ptr == NULL) {
				comparisonResult = 0;
			} else {
				comparisonResult = strcmp(nextToken, ptr->string);
			}
		}
		/*
		* If ptr is NULL, that means the end of the hashtable has been reached. In that case, insert a new key at the end with the name
		* of the next token and a new file hashtable object that contains just this file with frequency 1. 
		*/
		if (ptr == NULL) {
			printf("Reached end of hashtable\n");
			fileTable* newFile = malloc(sizeof(fileTable));
			newFile->fileName = nameOfFile;
			newFile->frequency = 1;
			newFile->next = NULL;
			stringTable* newString = malloc(sizeof(stringTable));
			newString->string = nextToken;
			newString->files = newFile;
			newString->next = NULL;
			if (prev != NULL) {
				prev->next = newString;
			} else {
				allStrings = newString;
			}
			/*
			* If the next token is already in the string hashtable, iterate through its file hashtable to see if this file is present there.
			* If it is, increment the frequency by one. Otherwise, create a new file key.
			*/
		} else if (comparisonResult == 0) {
			printf("Same string\n");
			fileTable* fileptr = ptr->files;
			fileTable* fileprev = NULL;
			int fileComparisonResult;
			if (fileptr == NULL) {
				fileComparisonResult = 0;
			} else {
				fileComparisonResult = strcmp(nameOfFile, fileptr->fileName);
			}
			while (fileComparisonResult > 0) {
				printf("iterate\n");
				fileprev = fileptr;
				fileptr = fileptr->next;
				if (fileptr == NULL) {
					fileComparisonResult = 0;
				} else {
					fileComparisonResult = strcmp(nameOfFile, fileptr->fileName);
				}
			}
			/*
			* If the end of the file hashtable is reached and the file was not found, create a new file key at the end.
			*/
			if (fileptr == NULL) {
				fileTable* newFile = malloc(sizeof(nameOfFile));
				newFile->fileName = nameOfFile;
				newFile->frequency = 1;
				newFile->next = NULL;
				if (fileprev != NULL) {
					fileprev->next = newFile;
				} else {
					ptr->files = newFile;
				}
			}
			/*
			* If the file is found, then increase its frequency. Because you didn't create a new file key, you can free nameOfFile.
			*/
			else if (fileComparisonResult == 0) {
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
				} else {
					ptr->files = newFile;
				}
			}
		/*
		* If the string value of the next token is greater than the string value of the current string in the hashtable, then the next token
		* is not in the hashtable. Create a new token key and create a new file hashtable for it.
		*/
		} else {
			printf("Inserting between nodes\n");
			fileTable* newFile = malloc(sizeof(fileTable));
			newFile->fileName = nameOfFile;
			newFile->frequency = 1;
			newFile->next = NULL;
			stringTable* newString = malloc(sizeof(stringTable));
			newString->string = nextToken;
			newString->files = newFile;
			newString->next = ptr;
			if (prev != NULL) {
				prev->next = newString;
			} else {
				allStrings = newString;
			}
		}
		// After checking for the given next token, get the next one.
		nextToken = getToken(file);
	}
	//Close the file at the end of everything.
	close(file);
}

int main(int argc, char *argv[]) {
	char* file = "Hints.txt";
	readFile(file);
	stringTable* ptr = allStrings;
	while (ptr != NULL) {
		printf("%s files: %s %d\n", ptr->string, ptr->files->fileName, ptr->files->frequency);
		ptr = ptr->next;
	}
}