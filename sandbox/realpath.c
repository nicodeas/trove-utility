// Example on how to use the function realpath
#include <limits.h> /* PATH_MAX */
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    char buf[PATH_MAX]; 
    char *res = realpath(argv[1], buf);
    if (res != NULL) { 
        printf("REAL PATH: %s \n", buf);
    } else {
        perror("realpath");
        exit(EXIT_FAILURE);
    }
    return 0;
}