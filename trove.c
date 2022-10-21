#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "trove.h"

#include "globals.h"
#include "build.h"
#include "update.h"
#include "find.h"
#include "remove.h"

#define OPTIONLIST "f:brul:"

void usage(char *name, char error)
{
    switch (error)
    {
    case '?':
        printf("Usage: %s [-f trovefile]  [-b  |  -r  |  -u]  [-l length]  filelist\n", name);
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
    printf("\t===== Arguments =====\n");
    printf("\tTrovefile: %s\n", trovefile);
    printf("\tb flag: %s\n", bflag ? "true" : "false");
    printf("\tr flag: %s\n", rflag ? "true" : "false");
    printf("\tu flag: %s\n", uflag ? "true" : "false");
    printf("\tLength: %d\n", word_length);
    argc -= optind;
    argv += optind;
    if (bflag)
    {
        HASHTABLE *hashtable = build_file(argv, argc, NULL);
        write_to_file(trovefile, hashtable);
    }
    else if (uflag)
    {
        HASHTABLE *hashtable = remove_file(trovefile, argv, argc);
        hashtable = build_file(argv, argc, hashtable);
        write_to_file(trovefile, hashtable);
    }
    else if (rflag)
    {
        HASHTABLE *hashtable = remove_file(trovefile, argv, argc);
        write_to_file(trovefile, hashtable);
    }
    else
    {
        find_word(trovefile, argv[0]);
    }
    exit(EXIT_SUCCESS);
}
