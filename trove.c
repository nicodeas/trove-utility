// CITS2002 Project 2 2022
// Student1: 22719855 MICHLIN NICHOLAS 50
// Student2: 23281104 KERR CHANTELLE 50
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
    printf("\t===== Arguments =====\n");
    printf("\tTrovefile: %s\n", trovefile);
    printf("\tb flag: %s\n", bflag ? "true" : "false");
    printf("\tr flag: %s\n", rflag ? "true" : "false");
    printf("\tu flag: %s\n", uflag ? "true" : "false");
    printf("\tLength: %d\n", word_length);
    argc -= optind;
    argv += optind;
    if (bflag && !uflag && !rflag)
    {
        HASHTABLE *hashtable = build_file(argv, argc, NULL);
        write_to_file(trovefile, hashtable);
    }
    else if (uflag && !bflag && !rflag)
    {
        HASHTABLE *hashtable = remove_file(trovefile, argv, argc);
        sanitise_hashtable(hashtable);
        hashtable = build_file(argv, argc, hashtable);
        write_to_file(trovefile, hashtable);
    }
    else if (rflag && !bflag && !uflag)
    {
        HASHTABLE *hashtable = remove_file(trovefile, argv, argc);
        sanitise_hashtable(hashtable);
        write_to_file(trovefile, hashtable);
    }
    else if (!rflag && !bflag && !uflag)
    {
        find_word(trovefile, argv[0]);
    }
    else
    {
        // There is multiple flags set
        usage("./trove", '?');
    }
    exit(EXIT_SUCCESS);
}
