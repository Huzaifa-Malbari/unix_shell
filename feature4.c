/* 
	Author: Huzaifa Malbari
	Email: huzaifamalbari@gmail.com
*/
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "feature4.h"
#include <errno.h>

// Used to store previouse directory path to enable
// cd -
char * previousDir = NULL;
int previousDirSize = 0;

/*
    Takes a pointer to a buffer and a pointer to its size.
    Sets the pointer to point to a string containing
    the path of the current directory and updates the size.
*/
void get_current_dir(char ** buffer, int * size) {
    
    if (*buffer == NULL) {
        *size = 50;
        *buffer = (char *) malloc(*size);
    }

    // Read the current directory path into buffer
    char * error = getcwd(*buffer, *size);
    while (error == NULL && errno == ERANGE) {
        *size += 50;
        *buffer = realloc(*buffer, *size);
        error = getcwd(*buffer, *size);
    }

}

/*
    Takes the raw path specified by the user and
    returns a pointer the absolute path used by the chdir syscall
*/
char * resolvePath(char * path) {

    if (strcmp(path, "-") == 0) {
        if (previousDir != NULL) {
            puts(previousDir);
        }
        return previousDir;
    }

    if (path[0] == '~') {
        char * home = getenv("HOME");
        char * fullPath = (char *) malloc(strlen(home) + strlen(path));
        strcpy(fullPath, home);
        strcat(fullPath, path + 1);
        return fullPath;
    }

    return path;
}

void change_dir(char * path) {

    char * fullPath = resolvePath(path);

    // Save current directory path to use as previousDir.
    char * oldDir = NULL;
    int oldDirSize = 0;
    get_current_dir(&oldDir, &oldDirSize);

    if (fullPath == NULL) {
        puts("cd: cannot retrieve previous directory");
        if (oldDir != NULL) {
            free(oldDir);
        }
        return;
    }

    int error = chdir(fullPath);

    if (error == -1) {
        // Construct error message to use with perror
        char * errorString = (char *) malloc(strlen(fullPath) + strlen("cd: ") + 1);
        strcpy(errorString, "cd: ");
        strcat(errorString, fullPath);
        perror(errorString);
        // Clean up
        free(errorString);
        free(oldDir);
        
    }else {
        previousDir = oldDir;
        previousDirSize = oldDirSize;
    }

    if (fullPath != path) {
        free(fullPath);
    }

}