#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>
#include <limits.h> /* PATH_MAX */
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

int word_length;

void convert_to_alpha(char *line)
{
    char *ptr = line;
    while (*ptr != '\0')
    {
        if (!isalpha(*ptr))
        {
            *ptr = ' ';
        }
        ++ptr;
    }
}

void process_line(char *line)
{
    char delimter[2] = {" "};
    char *token;
    token = strtok(line, delimter);
    while (token != NULL)
    {
        if (strlen(token) == word_length)
        {
            // Do something...
            printf("%s\n", token);
        }
        token = strtok(NULL, delimter);
    }
}

void parse_file(char *fname)
{
    FILE *fp = fopen(fname, "r");
    char line[BUFSIZ];
    if (fp == NULL)
    {
        perror(fname);
        exit(EXIT_FAILURE);
    }
    while (fgets(line, BUFSIZ, fp) != NULL)
    {
        convert_to_alpha(line);
        process_line(line);
    }
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
        // printf("==========Opening directory: %s ==========\n", file_arg);
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
            // printf("%s\n", dp->d_name);
            if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, ".."))
            {
                strcpy(path, base_path);
                strcat(path, "/");
                strcat(path, dp->d_name);
                parse_fileargs(path);
            }
        }
        // printf("==========Closing directory: %s ==========\n", file_arg);
        closedir(dirp);
    }
    else
    {
        parse_file(file_arg);
        // printf("%s\n", file_arg);
    }
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("%s: [wordlenght] [filename(s)] \n", argv[0]);
        exit(EXIT_FAILURE);
    }
    word_length = atoi(argv[1]);
    for (int i = 2; i < argc; i++)
    {
        parse_fileargs(argv[i]);
    }
    exit(EXIT_SUCCESS);
}
