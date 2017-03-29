#include "readAndWriteFile.h"
#include "dirFunctions.h"

static stringTable* allStrings;

/*
* This function checks whether the given path leads to a file or directory. It returns 1 if the given path leads
* to a directory, 0 if the given path leads to a file, and -1 otherwise.
*/
int isDir(char * path) {
        struct stat buf;
        stat(path, &buf);

		// Returns 1 if the path leads to a directory
        if(S_ISDIR(buf.st_mode) == 1) {
            return 1;
		// Returns 0 if the path leads to a file
        } else if(S_ISREG(buf.st_mode) == 1) {
            return 0;
		// Returns -1 otherwise
        } else {
            return -1;
        }

	return S_ISDIR(buf.st_mode);
}

/*
* This function simply reads each file in the directory. It takes the current path and file name and reads it.
* This function is meant to create a new stack for each read file so that the stack does not overflow.
*/
int multiple_files(char * path, char * fileName) {
	return readFile(path,fileName);
} 

/*
* This function searches the given directory recursively. It returns each file in the given directory and if another
* directory is found within, it calls search_dir on that directory recursively.
*/
void search_dir(char * dir) {
	
	fileUnit* root = NULL;
	fileUnit* trav = NULL;  

	int count = 0; 
	int index = 0;
	char * fileName; 
	
	struct dirent *entry;
	DIR * directory;
 
	directory = opendir(dir);
 
	/*
	* Iterate through the entire directory until the end is reached.
	*/
	while((entry = readdir(directory)) != NULL){  		
		fileName = entry->d_name;
		// Allocates memory to store the next path name to check
		char * next = (char*) malloc(2 + strlen(fileName) + strlen(dir));
		
		strcpy(next,dir);  
		
		if(strcmp(fileName, ".") && strcmp(fileName, "..")) {
			
			strcat(next,"/");
			strcat(next, fileName); 
		
			int result = isDir(next);
			
			//if directory, recurse and call this function again on that directory
			if(result == 1) { 
				search_dir(next);
				//if file, create a new fileUnit to hold that file
			} else if(result == 0) { 
				//Count all files in this subdirectory and create a new struct list. 
				count++;
				fileUnit* myfile = (fileUnit*)malloc(1 + sizeof(fileUnit)); 
				myfile->fileName = (char*)malloc(strlen(fileName));
				myfile->path = (char*) malloc(1 + strlen(fileName) + strlen(dir));
				
				strcpy(myfile->path, next); 
				strcpy(myfile->fileName, fileName);

				//Create linked list
				if(count <= 1) { 
					root = myfile;
					root->next = NULL;
					trav = root;
				} else {
					trav->next = myfile;
					trav = trav->next;
					trav->next = NULL; 					
				}
				
			}
		}	

		free(next);
	} 
	
	// At the very end, read the file linked list
	while(root != NULL) {
		multiple_files(root->path, root->fileName);
		root = root->next;
	}
	
	// Close the directory
	closedir(directory);	
}

/*
* This function returns the next token in the given file. It first mallocs 1000 bytes as a buffer to store the token as it is created.
* It goes through the file one char at a time, and starts when an alphabetical character is reached. If the end of file is reached
* before then, this function returns NULL. Otherwise, the token expands to contain all alphanumerical values afterwards and stops
* when it reaches a non-alphanumerical value. Then the function moves the buffer into a new token string malloced to be the size of the
* string. The buffer is then freed and the new token returned.
*/
char* getToken(int file) {
	int currentSize = 0;
	char* buffer = (char*)malloc(1000);
	memset(buffer,0,1000);
	
	char* nextChar = malloc(1);
	memset(nextChar, 0, 1);
	// Iterates through the file until an alphabetical character is reached.
	while (!isalpha(*nextChar)) {
		// If the end of the file is reached before another token is found, then return NULL.
		if (read(file, nextChar, 1) == 0) {
			return NULL;
		}
	}
	// Add all subsequent alphanumerical characters to the token.
	while (isalnum(*nextChar)) {
		buffer[currentSize] = tolower(*nextChar);
		currentSize++;
		read(file, nextChar, 1);
	}
	buffer[currentSize] = '\0';
	char* nextToken = (char*)malloc(currentSize + 1);
	memset(nextToken,0, currentSize+1);
	strncpy(nextToken, buffer, currentSize);
	nextToken[currentSize] = '\0';
	return nextToken;
}

/*
* This function takes the string representation of a file name and opens it and reads it. It opens the file in read mode 
* and iterates through one character at a time to get each string token. It adds these strings to the allStrings stringMap.
*/
int readFile(char* fileName, char* local_fileName) {

	int i = 0;
	/*
	* This stores the file in a new malloced object so that it can be retrieved later.
	*/
	char* nameOfFile = malloc(strlen(fileName) + 1);
	while (i < strlen(fileName)) {
		nameOfFile[i] = fileName[i];
		i++;
	}
	
	nameOfFile[i] = '\0';
	int file = open(fileName, O_RDONLY);
	/*
	* If the file does not exist, throw an error and return.
	*/
	if (file == -1) {
		printf("Specified file does not exist\n");
		return -1;
	}
	//Fetch the next token in the file. 
	char* nextToken = getToken(file);
	/*
	* Keep iterating through the tokens in the current file until it's NULL. This means that there are no more tokens
	* in the file.
	*/
	while (nextToken != NULL) {
		// Allocates memory and defines a new nameOfFile string to be stored if a new file key is created.
		nameOfFile = malloc(strlen(fileName) + 1);
		i = 0;
		while (i < strlen(fileName)) {
			nameOfFile[i] = fileName[i];
			i++;
		}
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
		// While the current pointer's string is less than the next token, keep advancing forward until you find where the token should be.
		while (comparisonResult > 0) {
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
			fileTable* newFile = (fileTable*) malloc(sizeof(fileTable));
		
			newFile->fileName = local_fileName;
			newFile->frequency = 1;
			newFile->next = NULL;
			newFile->prev = NULL;
			
			stringTable* newString = (stringTable*) malloc(sizeof(stringTable));
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
			fileTable* fileptr = ptr->files;
			fileTable* fileprev = NULL;
			int fileFound = 0;
			/*
			* Iterate through the entire file hashtable until the file is found or the end of the hashtable is reached.
			*/
			while (fileptr != NULL && fileFound == 0) {
				/* 
				* If the file name is found, increase frequency and then sort it so that it is in the correct position.
				*/

				if (strcmp(fileptr->fileName, local_fileName) == 0) { //local_fileName used to be nameOfFile
					(fileptr->frequency)++;
					free(nameOfFile);
					fileTable* newptr = fileptr;
					fileTable* newprev = NULL; 
					int positionFound = 0;
					/*
					* First, the fileptr is moved until it is in the appropriate frequency bracket. Then it is sorted 						
					* alphabetically so that it is in the correct alphabetical position within its frequency bracket.
					*/
					while (newptr != NULL && positionFound == 0) { // At this line newptr should equal fileptr
					
						//First conditional deals with same frequency but different names. newptr->fileName comes 
						//before 
						if (newptr->frequency == fileptr->frequency && strcmp(newptr->fileName, fileptr->fileName) < 0) {
							positionFound = 1;
							// If the frequency of the ptr is greater than
						}else if (newptr->frequency > fileptr->frequency) {  
							positionFound = 1;
							//This loop will run at the beginning of a new token.
						} else { 
							newprev = newptr;
							newptr = newptr->prev;
						}
					}
					// If a new position to move the file in the linked list is found, then do some pointer arithmetic.
					if (positionFound == 1) {
						if (fileptr->next != NULL) {
							if (fileptr->prev != fileptr) {
								fileptr->next->prev = fileptr->prev;
							}
						}
						if (fileptr->prev != NULL) {
							if (fileptr->next != fileptr) {
								fileptr->prev->next = fileptr->next;
							}
						}
						if (fileptr != newptr) {
							if (newptr!= NULL) {
								newptr->next = fileptr;
							}
							fileptr->prev = newptr;
						}
						if (fileptr != newprev) {
							if (newprev != NULL) {
								newprev->prev = fileptr;
							}
							fileptr->next = newprev;
						}
						/*
						* If a position is not found, then that means that this file has the highest frequency for this
						* token. Do some pointer arithmetic and set this equal to the first file in the token's file hashtable.
						*/
					} else {
						if (fileptr->next != NULL) {
							if (fileptr->prev != fileptr) {
								fileptr->next->prev = fileptr->prev;
							}
						}
						if (fileptr->prev != NULL) {
							if (fileptr->next != fileptr) {
								fileptr->prev->next = fileptr->next;
							}
						}
						if (fileptr != newprev) {
							if (newprev != NULL) {
								newprev->prev = fileptr;
							}
							fileptr->next = newprev;
						}
						fileptr->prev = NULL;
						ptr->files = fileptr;
					}
					fileFound = 1;
				}
				fileprev = fileptr;
				fileptr = fileptr->next;
			}
			/*
			* If the file wasn't found, create a new file key and append it to the end of the file hashtable for that token. Then sort it so
			* that it is in the correct alphabetical location.
			*/
			if (fileFound == 0) {
				fileptr = fileprev;
				fileprev = NULL;
				fileTable* newFile = malloc(sizeof(fileTable));
				newFile->fileName = local_fileName;
				newFile->frequency = 1;
				int positionFound = 0;
				while (fileptr != NULL && positionFound == 0) {
					if (fileptr->frequency == 1 && strcmp(fileptr->fileName, newFile->fileName) < 0) {
						positionFound = 1;
					}
					else if (fileptr->frequency > 1) {
						positionFound = 1;
					} else {
						fileprev = fileptr;
						fileptr = fileptr->prev;
					}
				}
				newFile->prev = fileptr;
				newFile->next = fileprev;
				if (fileprev != NULL) {
					fileprev->prev = newFile;
				}
				if (fileptr != NULL){
					fileptr->next = newFile;
				} else {
					ptr->files = newFile;
				}
			}
			
		/*
		* If the string value of the next token is greater than the string value of the current string in the hashtable, then the next token
		* is not in the hashtable. Create a new token key and create a new file hashtable for it.
		*/
		} else {
			fileTable* newFile = malloc(sizeof(fileTable));
			newFile->fileName = local_fileName;
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
	return 1;
}

/*
* This function writes to a new file with the given fileName using the given hashtable. The file will be formatted like an XML
* file.
*/
int writeFile(char* fileName, stringTable* hashtable) {
	/*
	* Create a new file in read or write mode with all permissions.
	*/
	int file = open(fileName, O_CREAT | O_WRONLY, S_IRUSR | S_IRGRP | S_IROTH);
	/*
	* Create a hashtable ptr to iterate through the string hashtable. Also include a nextLine string that will be written into
	* the file and change its value to change what will be written into the file.
	*/
	stringTable* ptr = hashtable;
	char* nextLine = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
	write(file, nextLine, strlen(nextLine));
	nextLine = "<fileIndex>\n";
	write(file, nextLine, strlen(nextLine));
	/*
	* Iterate through the hashtable until the end is reached. For each node in the hashtable, create the appropriate XML tags.
	*/
	while (ptr != NULL) {
		nextLine = malloc(strlen(ptr->string) + 100);
		sprintf(nextLine, "\t<word text=\"%s\">\n", ptr->string);
		write(file, nextLine, strlen(nextLine));
		free(nextLine);
		/*
		* After printing out the tags for the word, print out the tags for each file and the word's frequency in that file.
		*/
		fileTable* fileptr = ptr->files;
		while (fileptr != NULL) {
			nextLine = malloc(strlen(fileptr->fileName) + 100);
			sprintf(nextLine, "\t\t<file name=\"%s\">%d</file>\n", fileptr->fileName, fileptr->frequency);
			write(file, nextLine, strlen(nextLine));
			fileptr = fileptr->next;
			free(nextLine);
		}
		/*
		* Finally, print the closing tags for that word and move on to the next word.
		*/
		nextLine = "\t</word>\n";
		write(file, nextLine, strlen(nextLine));
		ptr = ptr->next;
	}
	/*
	* After printing out all the word and file tags, print the closing tags for the XML file.
	*/
	nextLine = "</fileIndex>\n";
	write(file, nextLine, strlen(nextLine));
	close(file);
	return 1;
}

/*
* This function frees everything that was malloced in this code.
*/
void freeAll() {
	stringTable* str_table;	
	stringTable* str_next; 
	fileTable* ptr; 
	fileTable* next; 

	ptr = allStrings->files;
	str_table = allStrings;

	// For each token...
	while(str_table != NULL) {
		// For each fileTable object, free it
		while(ptr != NULL){
			next = ptr->next;
			free(ptr); 
			ptr = next;
		}
		// Free each token after freeing its file hashtable
		str_next = str_table->next;
		free(str_table);
		str_table = str_next;
	}
}

int main(int argc, char *argv[]) {

	// If there aren't enough arguments, then exit the program.
	if(argc != 3) {
		printf("You did not pass enough arguments. Please indicate inverted_index file and target file/directory. Exiting now..\n"); 
		return;
	}	

	char * outputFile = argv[1]; 
	char * searchTarget = argv[2]; 

	char * input = (char*)malloc(1000);
	char * newName = (char*)malloc(1000); 

	// Tests what the name of the output file corresponds to; already existing directory, already existing file, or new file
	int result = isDir(outputFile); 	
	
	// If the output file is a directory, exit
	if(result == 1) {
		printf("Your output file matches the same name as an existing directory. Exiting now...\n"); 
		return;
	}

	// If the output file already exists, ask if you want to overwrite it
	if(result == 0) {
		printf("An output file or with same the name already exists...\n");
		printf("Would you like to override the file...?\n"); 
		printf("Enter 'y' to override or 'n' to choose another name or press Ctrl-C to exit the program...\n");
 			
		// Keep looping until break
		while(1) {
			scanf("%s",input); 
			// If the input is 'y' or 'n', continue, otherwise ask for input again
			if(!strcmp(input, "y") || !strcmp(input, "n")) {
				// If input is 'y', delete old file and replace it with a new one
				if(!strcmp(input, "y")) {
					printf("Overriding...\n");
					remove(outputFile);
					break; 	
					// If input is 'n', ask for a new name
				} else if (!strcmp(input, "n")) {
					while(1) {
						printf("Choose a new name for your file...\n");
						scanf("%s", newName);

						// If the given name also exists, ask for another name
						if(isDir(newName) == -1) {
							outputFile = newName; 						
							break; 	
						} else {
							printf("%s\n","A file/directory with that name already exists. Please try again...");
						}
					} 
					break;					   
				}
			} else {
				printf("Invalid input. Please reply with 'y' or 'n'...\n"); 
			}
		}
	}

	result = isDir(searchTarget);
		
	// Depending on whether the input file is a directory or file, either recurse or read it
	if(result == 1) {
		search_dir(searchTarget); 	
	} else if (result == 0) {
		readFile(searchTarget,searchTarget); 
	} else {
		printf("Invalid second argument. Ending program...\n"); 
	}
	// At the end, write everything to file and free it
	writeFile(outputFile, allStrings);
	freeAll();
	return 1;
}