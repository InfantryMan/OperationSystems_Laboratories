#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int scanfolder(int, char*);
int diag(char*);

int diag(char* mes) {
    write(1, mes, strlen(mes));
    write(1, ": ", 2);
    write(1, sys_errlist[errno], strlen(sys_errlist[errno]));
    write(1, "\n", 1);
    exit(errno);
}

int scanfolder(int type, char* suf) {
DIR* fdir;
DIR* fdirIter;
struct dirent* folder;
struct dirent* folderIter;
struct stat sbuf[1];
char* start;
int count=0;
int len;

if((fdir = opendir(".")) == NULL)
    return(errno);

while((folder = readdir(fdir)) != NULL) { 
    stat(folder->d_name, sbuf);
    if(((sbuf->st_mode) & S_IFMT) != type)
    	continue;
    if ((fdirIter = opendir(folder->d_name)) == NULL) 
    	return(errno);
    if (strcmp(folder->d_name, ".") && strcmp(folder->d_name, "..")) {
		folderIter = readdir(fdirIter);
		folderIter = readdir(fdirIter);
		if ((folderIter = readdir(fdirIter)) == NULL) {
		    len = strlen(folder->d_name);
		    write(1, folder->d_name, len + 1);
   			write(1, "\n", 2);
		}

    }
}    
	closedir(fdir);
	closedir(fdirIter);
	return(0);
}

int main(int argc, char* argv[]) {
	if(chdir(argv[1]) < 0)
		return(diag(argv[1]));  
	if(scanfolder(S_IFDIR, "/\n") > 0)
		return(diag(argv[1]));  
	return(0);
}

