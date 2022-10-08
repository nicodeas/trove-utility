// Example on how to use the function realpath
#include <limits.h> /* PATH_MAX */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

int is_dir(char *path)
{
    struct stat file_stat;
    stat(path, &file_stat);
    return S_ISREG(file_stat.st_mode);
}

int main(int argc, char *argv[])
{
    char path[PATH_MAX];
    if (argc < 2)
    {
        printf("Incorrect arguments\n");
        exit(EXIT_FAILURE);
    }
    char *res = realpath(argv[1], path);
    if (res != NULL)
    {
        printf("REAL PATH: %s \n", path);
    }
    else
    {
        perror("realpath");
        exit(EXIT_FAILURE);
    }
    int x = is_dir(path);
    printf("%i\n", x);

    return 0;
}