/* 
	Author: Huzaifa Malbari
	Email: huzaifamalbari@gmail.com
*/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "feature5.h"

int ORIGINAL_STDOUT = -1;

/*
    Takes the full command line entered by the user,
    checks if there is a redirect and if there is, handle
    it.
*/
int process_redirect(char * str) {

    int index = -1;

    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '>') {
            index = i;
            break;
        }
    }

    if (index == -1) {
        return index;
    }


    // Used strtok starting from after the '>' to get the filename
    char * filename = strtok(str + index + 1, " ");
    int fd = open(filename, O_WRONLY|O_CREAT|O_TRUNC, 0666);
    if (fd == -1) {
        perror("Error opening file");
        return -1;
    }

    // Set the position of '>' in the original command line
    // to the null character so that get_argv in feature1.c
    // ignores everything from the '>' character.
    str[index] = '\0';

    // Save original fd of stdout to a new fd to restore stdout later
    if (ORIGINAL_STDOUT == -1) {
        ORIGINAL_STDOUT = dup(1);
    }
    dup2(fd, 1);
    return fd;

}

// Close the redirect file's fd and reset stdout's fd
void cleanup_redirect(int fd) {
    close(fd);
    dup2(ORIGINAL_STDOUT, 1);
}