/* 
	Author: Huzaifa Malbari
	Email: huzaifamalbari@gmail.com
*/
#include <time.h>
#include <stdio.h>
#include "feature2.h"

void print_prompt(void) {

    time_t t = time(NULL);

    if (t == -1) {
        perror("Error getting time");
    }

    // Generate and get pointer to broken down time struct
    struct tm * ptime_struct = localtime(&t);

    char format[] = "[%d/%m %k :%M]# ";
    char prompt[sizeof(format)];

    // Extract required information from broken down time struct
    // for the specified prompt
    if (strftime(prompt, sizeof(format), format, ptime_struct) == 0) {
        fprintf(stderr, "strftime returned 0\n");
        prompt[0] = '#';
        prompt[1] = ' ';
        prompt[2] = '\0';
    }

    printf("%s", prompt);

}