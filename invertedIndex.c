#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>


typedef struct _fileTable{
	char* fileName; 
	int frequency; 
	struct _fileTable* next;
} fileTable; 

typedef struct _stringtable {
	char* string;
	fileTable* files;
	struct _stringTable* next;
} stringTable; 

static fileTable* all_files; 


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

void search_dir(char * dir) {
	
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
		char * next = (char*) malloc(1 + strlen(fileName) + strlen(dir));
		
		strcpy(next,dir);  
		
		if(strcmp(fileName, ".") && strcmp(fileName, "..")) {
			
			strcat(next,"/");
			strcat(next, fileName); 
			printf("%s\n", next); 
		
			int result = isDir(next);
			printf("%d\n", result); 
			
			if(result == 1) {
				search_dir(next);
			}
		}	

		free(next);
		printf("Hello\n");			

		printf("End of loop\n"); 
	} 

	closedir(directory);	
}

void parse_file(char * file) {

}

int main(int argc, char ** argv) {
	
	if(argc < 3) {
		printf("You did not pass enough arguments. Please indicate inverted_index file and target file/directory...\n"); 
	}	
	char * outputFile = argv[1]; 
	char * searchTarget = argv[2]; 
	
	int result = isDir(searchTarget); 

	if(result == 1) {
		printf("IS DIRECTORY\n");
		search_dir(searchTarget); 
	} else if(result == 0) {
		printf("IS FILE\n");
	} else {
		printf("Given target does not exist...\n");
	}
		
	return 0; 
}
