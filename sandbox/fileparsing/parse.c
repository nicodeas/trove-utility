#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>

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

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("%s: [filename] [wordlength]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    char *fname = argv[1];
    word_length = atoi(argv[2]);
    parse_file(fname);
    exit(EXIT_SUCCESS);
}
