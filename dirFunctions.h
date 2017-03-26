#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>

int isDir(char * path);
void search_dir(char * dir);