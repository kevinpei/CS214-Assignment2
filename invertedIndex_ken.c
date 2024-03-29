#include "readAndWriteFile.h"
#include "dirFunctions.h"

typedef struct fileUnit {
	char * fileName;
	char * path;
	struct fileUnit* next;
} happywheel;

static stringTable* allStrings;

/*
* This function returns the next token in the given file. It first mallocs 5 bytes and keeps track of the size of the token. 
*/
int isDir(char * path) {
        struct stat buf;
        stat(path, &buf);

        if(S_ISDIR(buf.st_mode) == 1) {
                return 1;
        } else if(S_ISREG(buf.st_mode) == 1) {
                return 0;
        } else {
                return -1;
        }

	return S_ISDIR(buf.st_mode);
}

/*
*/

void multiple_files(char * path, char * fileName) {
	return readFile(path,fileName);
} 

void search_dir(char * dir) {
	
	happywheel* root = NULL;
	happywheel* trav = NULL;  

	int count = 0; 
	int index = 0;
	char * fileName; 
	
	printf("Here...\n");
	struct dirent *entry;
	DIR * directory;

	if((directory = opendir(dir)) == NULL) {
		printf("Do this\n");
	} 
 
	while((entry = readdir(directory)) != NULL){  		
		printf("Enter loop\n");
		fileName = entry->d_name;
		printf("%s\n", "I'm looking at" + *fileName);
		char * next = (char*) malloc(2 + strlen(fileName) + strlen(dir));
		
		strcpy(next,dir);  
		
		if(strcmp(fileName, ".") && strcmp(fileName, "..")) {
			
			strcat(next,"/");
			strcat(next, fileName); 
			printf("%s\n", next); 
		
			int result = isDir(next);
			printf("%d\n", result); 
			
			if(result == 1) { //if directory
				search_dir(next);
			} else if(result == 0) { //if file
				//Count all files in this subdirectory and create a new struct list. 
				count++;
				happywheel* myfile = (happywheel*)malloc(1 + sizeof(happywheel)); 
				myfile->fileName = (char*)malloc(strlen(fileName));
				myfile->path = (char*) malloc(1 + strlen(fileName) + strlen(dir));
				
				strcpy(myfile->path, next); 
				strcpy(myfile->fileName, fileName);
				//printf("%s\n", myfile->fileName); 
				printf("Here...\n");
				printf("%d\n", count); 
				
				//Create linked list
				
				if(count <= 1) { 
					printf("Enter top...\n");
					root = myfile;
					printf("1\n");
					root->next = NULL;
					printf("2\n");
					trav = root;
					printf("Created...\n");  
				} else {
					printf("Enter bottom...\n");
					trav->next = myfile;
					trav = trav->next;
					trav->next = NULL; 					
				}
				
			}
		}	

		free(next);
		printf("Hello\n");			

		printf("End of loop\n"); 
	} 
	
	while(root != NULL) {
		printf("I'm in here...\n");
		printf("%s\n", root->fileName);
		multiple_files(root->path, root->fileName);
		root = root->next;
	}

	closedir(directory);	
}

/*
* This function returns the next token in the given file. It first mallocs 5 bytes and keeps track of the size of the token.
* It goes through the file one char at a time, and starts when an alphabetical character is reached. If the end of file is reached
* before then, this function returns NULL. Otherwise, the token expands to contain all alphanumerical values afterwards and stops
* when it reaches a non-alphanumerical value. Then the function returns that token.
*/
char* getToken(int file) {
	int currentSize = 0;
	char* buffer = malloc(1000);
	
	char* nextChar = malloc(1);
	// Iterates through the file until an alphabetical character is reached.
	while (!isalpha(*nextChar)) {
		// If the end of the file is reached before another token is found, then return NULL.
		if (read(file, nextChar, 1) == 0) {
			printf("End of file\n");
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
	char* nextToken = malloc(currentSize + 1);
	strncpy(nextToken, buffer, currentSize);
	nextToken[currentSize] = '\0';
	return nextToken;
}

/*
* This function takes the string representation of a file name and opens it and reads it. It opens the file in read mode 
* and iterates through one character at a time to get each string token. It adds these strings to the allStrings stringMap.
*/
int readFile(char* fileName, char* local_fileName) {
	
	printf("READING\n");
	printf("%s\n", "Local name is: ");
	printf(local_fileName);
	printf("%s\n", "Path is: ");
	printf(fileName);

	int i = 0;
	/*
	* This converts the file name to all lower case.
	*/
	char* nameOfFile = malloc(strlen(fileName) + 1);

	while (i < strlen(fileName)) {
		printf("Changing character %d\t", i);
		nameOfFile[i] = tolower(fileName[i]);
		i++;
	}
	
	nameOfFile[i] = '\0';
	printf("Opening %s\n", fileName);
	int file = open(fileName, O_RDONLY);
	printf("%s opened\n", nameOfFile);
	/*
	* If the file does not exist, throw an error and return.
	*/
	if (file == -1) {
		printf("Specified file does not exist\n");
		return -1;
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
			nameOfFile[i] = tolower(fileName[i]);
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
			printf("Reached end of hashtable\n");
			fileTable* newFile = (fileTable*) malloc(sizeof(fileTable));
		
			newFile->fileName = local_fileName;
			newFile->frequency = 1;
			newFile->next = NULL;
			newFile->prev = NULL;
			
			stringTable* newString = malloc(sizeof(stringTable));
			newString->string = nextToken;
			newString->files = newFile;
			newString->next = NULL;
			printf("World goes boom \n");
			printf("%s\n",newString->files->fileName);
			
			if (prev != NULL) {
				prev->next = newString;
			} else {
				allStrings = newString;
			}
			printf("What's happening\n");
			/*
			* If the next token is already in the string hashtable, iterate through its file hashtable to see if this file is present there.
			* If it is, increment the frequency by one. Otherwise, create a new file key.
			*/
		} else if (comparisonResult == 0) {
			//printf("Same string\n");
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
				printf("Crash here?\n");
				


				if (strcmp(fileptr->fileName, local_fileName) == 0) { //local_fileName used to be nameOfFile
					printf("THIS\n");
					printf("%s\n",fileptr->fileName); 
					printf("%s\n", local_fileName);
					(fileptr->frequency)++;



					printf("%s\n","Frequency is: " + fileptr->frequency);
					free(nameOfFile);
					fileTable* newptr = fileptr;
					//fileTable* newprev = NULL; 
					//THIS LINE causes segfault. 
					fileTable* newprev = (fileTable*) malloc(sizeof(fileTable));
					int positionFound = 0;
					/*
					* First, the fileptr is moved until it is in the appropriate frequency bracket. Then it is sorted 						* alphabetically so that
					* it is in the correct alphabetical position within its frequency bracket.
					*/

					//THERE IS SOMETHING WRONG WITH THIS WHILE LOOP							
					printf("Before loop...\n");
					
					while (newptr != NULL && positionFound == 0) { // At this line newptr should equal fileptr
						//printf("Looping...");						
						//printf("%s\n",newptr->frequency);						

						//First conditional deals with same frequency but different names. newptr->fileName comes 
						//before 
						if (newptr->frequency == fileptr->frequency && strcmp(newptr->fileName, fileptr->fileName) < 0) {
							printf("Compared %s and %s\n", newptr->fileName, fileptr->fileName);
							printf("Found correct bracket\n");
							positionFound = 1;
							printf("Block 1...\n");
						}else if (newptr->frequency > fileptr->frequency) { // If the frequency of the ptr is greater than 
							printf("Missed bracket\n");
							positionFound = 1;
							printf("Block 2...\n");
						} else { //This loop will run at the beginning of a new token.
							newprev = newptr;
							newptr = newptr->prev;

							if(newptr != NULL) {
								printf("Well shit..."); 
							}

							printf("Block 3...\n");
						}
					}


					printf("Maybe...\n");
					if (positionFound == 1) {
						printf("Found a position\n");
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
						printf("prev is %s\n", newptr->fileName);
						printf("This is %s\n", fileptr->fileName);
						if (fileptr != newprev) {
							if (newprev != NULL) {
								newprev->prev = fileptr;
							}
							fileptr->next = newprev;
						}
					} else {
						printf("Reached the end\n");
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
				printf("File not found\n");
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
					printf("Changing the next file\n");
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
	// If file isn't -1, that means a file with the same name already exists.
/*	if (file != -1) {
		printf("This file already exists.\n");
		close(file);
		return -1;
	}
	/*
	* Create a hashtable ptr to iterate through the string hashtable. Also include a nextLine string that will be written into
	* the file and change its value to change what will be written into the file.
	*/
	stringTable* ptr = hashtable;
	char* nextLine = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
	printf("Writing 1\n");
	write(file, nextLine, strlen(nextLine));
	nextLine = "<fileIndex>\n";
	printf("Writing 2\n");
	write(file, nextLine, strlen(nextLine));
	/*
	* Iterate through the hashtable until the end is reached. For each node in the hashtable, create the appropriate XML tags.
	*/
	while (ptr != NULL) {
		nextLine = malloc(strlen(ptr->string) + 100);
		sprintf(nextLine, "\t<word text=\"%s\">\n", ptr->string);
		printf("Writing %s\n", ptr->string);
		write(file, nextLine, strlen(nextLine));
		free(nextLine);
		/*
		* After printing out the tags for the word, print out the tags for each file and the word's frequency in that file.
		*/
		fileTable* fileptr = ptr->files;
		while (fileptr != NULL) {
			nextLine = malloc(strlen(fileptr->fileName) + 100);
			printf("WOOOOOO\n");
			printf("%s\n", fileptr->fileName);
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

void freeAll() {
	stringTable* str_table;	
	stringTable* str_next; 
	fileTable* ptr; 
	fileTable* next; 

	ptr = allStrings->files;
	str_table = allStrings;

	while(str_table != NULL) {
		while(ptr != NULL){ //For one string
			next = ptr->next;
			free(ptr); 
			ptr = next;
		}
		str_next = str_table->next;
		free(str_table);
		str_table = str_next;
	}
	printf("Freed all\n"); 
}

int main(int argc, char *argv[]) {

	if(argc < 3) {
		printf("You did not pass enough arguments. Please indicate inverted_index file and target file/directory...\n"); 
	}	

	char * outputFile = argv[1]; 
	char * searchTarget = argv[2]; 
	
	int result = isDir(outputFile); 	
	
	if(result == 1) {
		printf("Your output file matches the same name as an existing directory. Exiting now...\n"); 
		return;
	}

	if(result == 0) {
		char * input = (char*)malloc(1000);
		char * newName = (char*)malloc(1000); 

		printf("An output file or with same the name already exists...\n");
		printf("Would you like to override the file...?\n"); 
		printf("Enter 'y' to override or 'n' to choose another name or press Ctrl-C to exit the program...\n");
 			
		while(1) {
			scanf("%s",input); 
			if(!strcmp(input, "y") || !strcmp(input, "n")) {
				if(!strcmp(input, "y")) {
					printf("Overriding...\n");
					remove(outputFile);
					//writeFile(outputFile, allStrings);
					break; 	
				} else if (!strcmp(input, "n")) {
					while(1) {
						printf("Choose a new name for your file...\n");
						scanf("%s", newName);
					
						//int x = isDir(newName);
						printf("%d\n", newName); 

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
		
	if(result == 1) {
		search_dir(searchTarget); 	
	} else if (result == 0) {
		printf("IS FILE");
		readFile(searchTarget,searchTarget); 
	} else {
		printf("Invalid second argument. Ending program...\n"); 
	}

	stringTable* ptr = allStrings;
	while (ptr != NULL) {
		printf("%s files: %s %d\n", ptr->string, ptr->files->fileName, ptr->files->frequency);
		ptr = ptr->next;
	}

	writeFile(outputFile, allStrings);
	freeAll();
	return 1;
}
