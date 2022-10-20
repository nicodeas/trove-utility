#include "globals.h"
#include "build.h"

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

void process_line(char *line, char *path, HASHTABLE *hashtable)
{

    char delimter[2] = {" "};
    char *token;
    token = strtok(line, delimter);
    while (token != NULL)
    {
        if (strlen(token) >= word_length)
        {

            HEAD_LINK *head = hashtable_find(hashtable, token);
            if (head == NULL)
            {
                head = hashtable_add(hashtable, token);
            }

            LINK *ltp_cpy = head->link_to_paths;
            if (!path_link_find(ltp_cpy, path))
            {
                LINK *path_link = new_link(path);
                path_link->next = head->link_to_paths;
                head->link_to_paths = path_link;
            }
        }
        token = strtok(NULL, delimter);
    }
}

void parse_file(char *fname, char *path, HASHTABLE *hashtable)
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
        process_line(line, path, hashtable);
    }
}

void parse_fileargs(char *file_arg, HASHTABLE *hashtable)
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
                parse_fileargs(path, hashtable);
            }
        }
        closedir(dirp);
    }
    else
    {
        parse_file(file_arg, base_path, hashtable);
    }
}

void write_to_file(char *filename, HASHTABLE *hashtable)
{
    int fd[2];
    int terminal_output = dup(STDOUT_FILENO);
    if ((pipe(fd) == -1))
    {
        printf("Failed to create pipe!\n");
        exit(EXIT_FAILURE);
    }
    switch (fork())
    {
    case -1:
        printf("Failed to fork!\n");
        exit(EXIT_FAILURE);
        break;
    case 0:
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        // Write to trovefile instead of stdout
        int output_fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
        dup2(output_fd, STDOUT_FILENO);

        if (execl("/usr/bin/gzip", "gzip", NULL) == -1)
        {
            perror("error");
        }
        break;

    default:
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        for (int i = 0; i < HASHTABLE_SIZE; i++)
        {
            while (hashtable[i] != NULL)
            {
                printf("#%s\n", hashtable[i]->word);
                LINK *links = hashtable[i]->link_to_paths;
                while (links != NULL)
                {
                    printf("%s\n", links->path);
                    links = links->next;
                }
                hashtable[i] = hashtable[i]->next;
            }
        }
        close(fd[1]);
        dup2(terminal_output, STDOUT_FILENO);
        break;
    }
}

void build_file(char *file_list[], char *filename, int file_count)
{
    HASHTABLE *hashtable = hashtable_new();
    for (int i = 0; i < file_count; i++)
    {
        parse_fileargs(file_list[i], hashtable);
    }
    printf("=====File Indexing Complete!\n");
    printf("=====Writing and Compressing to file=====\n");
    write_to_file(filename, hashtable);
    printf("=====Compression Complete!=====\n");
}
