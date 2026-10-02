/* 
	Author: Huzaifa Malbari
	Email: huzaifamalbari@gmail.com
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "feature1.h"
#include "feature2.h"
#include "feature3.h"
#include "feature4.h"

extern char * previousDir;
extern int previousDirSize;

// Used as a flag for SIGINT handler to determine
// whether to print prompt again or not
int bPrintPrompt = 1;

void start_shell() {

	get_current_dir(&previousDir, &previousDirSize);

	char * line = NULL;
	size_t size = 0;

	while (!feof(stdin)) {

		print_prompt();

		size = getline(&line, &size, stdin);
		if (size == -1) {
			if (feof(stdin)) {
				puts("");
				exit(EXIT_SUCCESS);
			}
			perror("error reading line from stdin");
			exit(EXIT_FAILURE);
		}
		line[size - 1] = '\0';

		int err = run_command(line);

	}

	free(line);
	exit(EXIT_SUCCESS);

}

int main() {
	
	catch_sig_int();
	start_shell();

	return 0;
}
