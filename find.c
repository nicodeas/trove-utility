#include <stdio.h>
#include <stdlib.h>
#include "find.h"

FILE *get_file(char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        perror(filename);
    }
    return fp;
}
void find_word(FILE *trovefile, char *word)
{
    printf("finding word:%s\n", word);
    char line[BUFSIZ];
    bool found = false;
    while (fgets(line, BUFSIZ, trovefile) != NULL)
    {
        char *curr = line;
        line[strlen(line) - 1] = '\0';
        curr++;
        if (*line == '#')
        {
            if (found)
            {
                break;
            }
            if (strcmp(word, curr) == 0)
            {
                found = true;
                continue;
            }
        }
        if (found)
        {
            printf("%s\n", line);
        }
    }
}
