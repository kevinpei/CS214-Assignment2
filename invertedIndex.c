#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

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
	
	char * fileName; 
	DIR * directory = opendir(dir); 
	
	struct dirent * entry; 

	while(1) {
		entry = readdir(directory);  
		if(entry != NULL) {
			fileName = entry->d_name;
						

			printf(fileName);
			printf("\n");
		} else {
			break; 
		}
	} 

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
