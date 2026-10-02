/* 
	Author: Huzaifa Malbari
	Email: huzaifamalbari@gmail.com
*/
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include "feature1.h"
#include "feature4.h"
#include "feature5.h"

extern int bPrintPrompt;

/*
	Takes a string and splits it into and returns
	an array expected by argv using strtok with delimiter " "
*/
char ** get_argv(char * str) {

	char * token = NULL;
	int argc = 1;
	char * *argv = malloc(sizeof(char *) * argc + 1);
	argv[1] = NULL;

	// keeps track of the index where strtok has currently processed to;
	// Used to treat double quoted strings as 1 argument
	int currIndex = 0;
	int strSize = strlen(str);
	
	token = strtok(str, " ");
	argv[argc - 1] = token;

	if (token == NULL) {
		return argv;
	}
	currIndex += strlen(token) - 1;
	token = strtok(NULL, " ");
	
	int skipToken = 0;
	while (token != NULL) {

		/* 	
			Used to treat strings enclosed with double quotes
			as a single argument. This is done by ignoring strtok's
			output, finding the ending double quote and setting 
			token to point to this manually calculated token (the string
			enclosed in double quotes)
		*/ 
		if (currIndex + 2 < strSize) {
			if (str[currIndex + 2] == '"') {
				int j = currIndex + 3;
				while (j < strSize && str[j] != '"') {
					if (str[j] == '\0') {
						str[j] = ' ';
					}
					j++;
				}
				if (str[j] == '"') {
					str[j] = '\0';
					token = str + currIndex + 3;
					skipToken = 1;
				}
			}
		}

		currIndex += strlen(token) + 2;

		argc++;
		argv = realloc(argv, sizeof(char *) * argc + 1);
		argv[argc - 1] = token;

		// If token is a double quoted string, restart strok from after
		// the toekn
		if (skipToken) {
			skipToken = 0;
			if (currIndex + 2 < strSize && str[currIndex + 2] != '\0') {
				token = strtok(str + currIndex + 2, " ");
			}else {
				break;
			}
		}else {
			token = strtok(NULL, " ");
		}
	}
	argv[argc] = NULL;

	return argv;
}

/*
	Takes the command line entered by the user and calls
	helper functions to process the redirect, handle built in cd
	or run command as a child process using fork and execvp
*/
int run_command(char * command) {

	int fd = process_redirect(command);

	char ** argv = get_argv(command);	
	if (argv[0] == NULL) {
		return -1;
	}

	// Call built in cd command
	if (strcmp(argv[0], "cd") == 0) {
		if (argv[1] == NULL) {
			change_dir("~");
		}else {
			change_dir(argv[1]);
		}
		return 0;
	}
	
	// Run command specified by user as child process
	pid_t child = fork();

	if (child == 0) {
	
		execvp(argv[0], argv);
		perror("Error executing command");
		exit(EXIT_FAILURE);
	
	}


	bPrintPrompt = 0;
	int status = 0;
	wait(&status);
	free(argv);

	// If there was a redirect, reset the stdout file descriptor
	// and close the other file descriptor
	if (fd >= 0) {
		cleanup_redirect(fd);
	}

	bPrintPrompt = 1;

	return status;

}
