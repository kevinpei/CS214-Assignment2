#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>

/*
* The fileUnit struct contains the name of a given file, its path, and a pointer to the next file in the
* list.
*/
typedef struct _fileUnit {
	char * fileName;
	char * path;
	struct _fileUnit* next;
} fileUnit;

int isDir(char * path);
int multiple_files(char * path, char * fileName);
void search_dir(char * dir);