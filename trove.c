#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
// #include "trove.h"

#include "globals.h"
#include "trovelib.h"

#define OPTIONLIST "f:brul:"

void usage(char *name, char error)
{
    switch (error)
    {
    case '?':
        printf("Usage: %s [-f trovefile]  [-b  |  -r  |  -u]  [-l length]  filelist\n", name);
        printf("Usage: %s [-f trovefile] word\n", name);
        break;
    }
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    char *trovefile = NULL;
    int opt;
    bool bflag, rflag, uflag;
    bflag = false;
    rflag = false;
    uflag = false;

    word_length = DEFAULT_LENGTH;
    trovefile = DEFAULT_TROVE_FILE_PATH;

    while ((opt = getopt(argc, argv, OPTIONLIST)) != -1)
    {
        switch (opt)
        {
        case 'f':
            trovefile = strdup(optarg);
            break;
        case 'b':
            bflag = true;
            break;
        case 'r':
            rflag = true;
            break;
        case 'u':
            uflag = true;
            break;
        case 'l':
            word_length = atoi(optarg);
            break;
        case '?':
            usage(argv[0], '?');
        }
    }
    printf("===== Arguments =====\n");
    printf("Trovefile: %s\n", trovefile);
    printf("b flag: %s\n", bflag ? "true" : "false");
    printf("r flag: %s\n", rflag ? "true" : "false");
    printf("u flag: %s\n", uflag ? "true" : "false");
    printf("Length: %d\n", word_length);
    argc -= optind;
    argv += optind;
    if (bflag)
    {
        // Calls the function on build.c
        build_file(argv, trovefile, argc);
    }
    else if (uflag)
    {
        // Calls the function on update.c
        update_file();
    }
    else if (rflag)
    {
        // Calls the function on remove.c
        // FILE *trove = get_file(trovefile);
        remove_file(trovefile, argv, argc);
    }
    else
    {
        // FILE *trove = get_file(trovefile);
        // printf("Searching for: %s \n", argv[0]);
        find_word(trovefile, argv[0]);
    }
    exit(EXIT_SUCCESS);
}
