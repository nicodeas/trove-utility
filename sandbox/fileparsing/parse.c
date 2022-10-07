#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

typedef struct _link
{
    char *paths;
    struct _list *link;

} LINK;

struct words
{
    char *word;
    struct _link *start_of_list;
};

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
        printf("%s\n", line);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("argument error\n");
        exit(EXIT_FAILURE);
    }
    char *fname = argv[1];
    printf("%s\n", fname);
    parse_file(fname);
    return 0;
}
