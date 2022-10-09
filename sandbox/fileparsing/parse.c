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
#include <fcntl.h>

typedef struct _link
{
    char *path;
    struct _link *next;
} LINK;

typedef struct _head
{
    char *word;
    struct _head *next;
    LINK *link_to_paths;

} HEAD_LINK;

HEAD_LINK *trove;

HEAD_LINK *find_word(char *word)
{
    HEAD_LINK *head = trove;
    while (head != NULL)
    {
        if (strcmp(head->word, word) == 0)
        {
            return head;
        }
        head = head->next;
    }
    return NULL;
}

HEAD_LINK *init_head(char *word)
{
    // add to front of list
    HEAD_LINK *new_head = malloc(sizeof(HEAD_LINK));
    new_head->word = strdup(word);
    new_head->next = trove;
    new_head->link_to_paths = NULL;
    trove = new_head;
    return trove;
}
LINK *init_link(char *path)
{
    LINK *new_link = malloc(sizeof(LINK));
    new_link->path = strdup(path);
    new_link->next = NULL;
    return new_link;
}

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

bool link_exists(HEAD_LINK *head, char *path)
{
    LINK *links = head->link_to_paths;
    while (links != NULL)
    {
        if (strcmp(links->path, path) == 0)
        {
            return true;
        }
        links = links->next;
    }
    return false;
}
void free_headlink(HEAD_LINK *head)
{
    // free paths separately
    free(head->word);
    free(head);
}
void free_path(LINK *path_link)
{
    free(path_link->path);
    free(path_link);
}

void process_line(char *line, char *path)
{
    char delimter[2] = {" "};
    char *token;
    token = strtok(line, delimter);
    while (token != NULL)
    {
        if (strlen(token) >= word_length)
        {
            // Do something...
            HEAD_LINK *head = find_word(token);
            if (head == NULL)
            {
                head = init_head(token);
            }
            if (head->link_to_paths == NULL)
            {
                head->link_to_paths = init_link(path);
            }
            else
            {
                if (!link_exists(head, path))
                {
                    LINK *path_link = init_link(path);
                    path_link->next = head->link_to_paths;
                    head->link_to_paths = path_link;
                }
            }
        }
        token = strtok(NULL, delimter);
    }
}

void parse_file(char *fname, char *path)
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
        process_line(line, path);
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
            if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, ".."))
            {
                strcpy(path, base_path);
                strcat(path, "/");
                strcat(path, dp->d_name);
                parse_fileargs(path);
            }
        }
        closedir(dirp);
    }
    else
    {
        parse_file(file_arg, base_path);
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
    int output_fd = open("./trove-data.txt", O_WRONLY);
    int terminal_output_copy = dup(STDOUT_FILENO);
    switch (fork())
    {
    case 0:
        dup2(output_fd, STDOUT_FILENO);
        while (trove != NULL)
        {
            HEAD_LINK *prev_head = trove;
            printf("#%s\n", trove->word);
            LINK *links = trove->link_to_paths;
            while (links != NULL)
            {
                LINK *prev = links;
                printf("%s\n", links->path);
                links = links->next;
                free_path(prev);
            }
            trove = trove->next;
            free_headlink(prev_head);
        }
        exit(EXIT_SUCCESS);
        break;
    default:
        wait(NULL);
        close(output_fd);
        dup2(terminal_output_copy, STDOUT_FILENO);
        break;
    }
    printf("Parsing Complete!\n");
    exit(EXIT_SUCCESS);
}
