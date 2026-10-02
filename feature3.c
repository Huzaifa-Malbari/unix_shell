/* 
	Author: Huzaifa Malbari
	Email: huzaifamalbari@gmail.com
*/
#include <signal.h>
#include <stdio.h>
#include "feature2.h"

extern int bPrintPrompt;

void handle_sin_int(int signal) {
    /*
        If bPrintPrompt is set, the signal was sent while
        waiting for user input, therefore prompt should be printed
        again

        Otherwise signal was sent while child process is running.
        The signal will kill the child process and print "^C" on
        the next line. Since the prompt will be printed by the next
        iteration of the shell, only a newline is printed here to
        ensure the prompt is printed on a freash line.
    */
    if (bPrintPrompt) {
        puts("");
        print_prompt();
        fflush(stdout);
    }else {
        puts("");
    }
}

void catch_sig_int(void) {
    signal(SIGINT, handle_sin_int);

}