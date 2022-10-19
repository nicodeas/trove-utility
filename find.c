#include <stdio.h>
#include <stdlib.h>
#include "find.h"

void find_word(char *trovefile, char *word)
{
    int fd[2];
    if (pipe(fd))
    {
        printf("Error creating pipe\n");
        exit(EXIT_FAILURE);
    };
    int pid = fork();
    if (pid == -1)
    {
        printf("Fork failed !!\n");
        exit(EXIT_FAILURE);
    }
    if (pid == 0)
    {
        dup2(fd[1], STDOUT_FILENO); // instead of writing to stdout, it writes to the pipe
        close(fd[0]);
        close(fd[1]);
        execl("/usr/bin/gzcat", "gzcat", trovefile, NULL);
    }
    close(fd[1]);
    FILE *stream;
    stream = fdopen(fd[0], "r");
    char line[BUFSIZ];
    bool found = false;
    while (fgets(line, BUFSIZ, stream) != NULL)
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
    close(fd[0]);
}
