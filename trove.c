#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#define DEFAULT_LENGTH 4
#define DEFAULT_TROVE_FILE_PATH "/tmp/trove"
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
    int opt, length;
    bool bflag, rflag, uflag;

    length = DEFAULT_LENGTH;
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
            length = atoi(optarg);
            break;
        case '?':
            usage(argv[0], '?');
        }
    }
    printf("=====Arguments=====\n");
    printf("Trovefile: %s\n", trovefile);
    printf("b flag: %s\n", bflag ? "true" : "false");
    printf("r flag: %s\n", rflag ? "true" : "false");
    printf("u flag: %s\n", uflag ? "true" : "false");
    printf("Length: %d\n", length);
    printf("=====End of Arguments=====\n");
    argc -= optind;
    argv += optind;
    printf("=====Files to include=====\n");
    for (int i = 0; i < argc; i++)
    {
        printf("%s\n", argv[i]);
    }

    exit(EXIT_SUCCESS);
}
