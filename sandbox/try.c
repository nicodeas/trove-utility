
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <getopt.h>

#define OPTLIST "df:n:"

int main(int argc, char *argv[])
{
    int opt;

    opterr = 0;
    while ((opt = getopt(argc, argv, OPTLIST)) != -1)
    {
        printf("Option = %c\n", opt);
        //  ACCEPT A BOOLEAN ARGUMENT
        if (opt == 'd')
        {
            printf("d flag!\n");
        }
        //  ACCEPT A STRING ARGUMENT
        else if (opt == 'f')
        {
            printf("%s\n", optarg);
        }
        //  ACCEPT A INTEGER ARGUMENT
        else if (opt == 'n')
        {
            printf("%s\n", optarg);
            // value = atoi(optarg);
        }
        //  OOPS - AN UNKNOWN ARGUMENT
        else
        {
            // perror(opterr);
            // printf("=======================unknownflag=================\n");
            argc = -1;
        }
    }

    // if (argc <= 0)
    // { //  display program's usage/help
    //     usage(1);
    // }
    argc -= optind;
    argv += optind;
    printf("=============remaining args=============\n");
    for (int i = 0; i < argc; i++)
    {
        printf("%s\n", argv[i]);
    }

    return 0;
}