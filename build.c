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

int build_file(char *file_list[], char *filename, int file_count)
{
    int output_fd = open(filename, O_WRONLY);
    int terminal_output_copy = dup(STDOUT_FILENO);
    FILE *fp = fopen(filename, "w");

    switch (fork())
    {
    case 0:
        dup2(output_fd, STDOUT_FILENO);
        HASHTABLE *hashtable = hashtable_new();
        for (int i = 0; i < file_count; i++)
        {
            parse_fileargs(file_list[i], hashtable);
        }
        for (int i = 0; i < HASHTABLE_SIZE; i++)
        {
            while (hashtable[i] != NULL)
            {
                printf("#%s\n", hashtable[i]->word);
                fprintf(fp, "#%s\n", hashtable[i]->word); // Write word to file
                LINK *links = hashtable[i]->link_to_paths;
                while (links != NULL)
                {
                    printf("%s\n", links->path);
                    fprintf(fp, "%s\n", links->path); // Write path to file
                    links = links->next;
                }
                hashtable[i] = hashtable[i]->next;
            }
        }
        // Compress the file
        execl("/usr/bin/gzip", "gzip", filename, NULL);
        exit(EXIT_SUCCESS);
        break;
    default:
        wait(NULL);
        fclose(fp);
        close(output_fd);
        dup2(terminal_output_copy, STDOUT_FILENO);
    }
    printf("parsing complete!\n");

    return 0;
}