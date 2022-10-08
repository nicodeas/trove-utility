#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "trove.h"
#include <limits.h> /* PATH_MAX */
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

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
void parse_fileargs(char *file_arg)
{
    char base_path[PATH_MAX];
    char path[PATH_MAX];
    struct stat file_stat;
    realpath(file_arg, base_path);
    stat(base_path, &file_stat);
    if (S_ISDIR(file_stat.st_mode))
    {
        printf("==========Opening directory: %s ==========\n", file_arg);
        DIR *dirp;
        struct dirent *dp;
        dirp = opendir(file_arg);
        if (dirp == NULL)
        {
            perror(file_arg);
            exit(EXIT_FAILURE);
        }
        while ((dp = readdir(dirp)) != NULL)
        {
            printf("%s\n", dp->d_name);
            if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, ".."))
            {
                strcpy(path, base_path);
                strcat(path, "/");
                strcat(path, dp->d_name);
                parse_fileargs(path);
            }
        }
        printf("==========Closing directory: %s ==========\n", file_arg);
        closedir(dirp);
    }
    else
    {
        printf("%s\n", file_arg);
    }
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
        parse_fileargs(argv[i]);
    }
    exit(EXIT_SUCCESS);
}
