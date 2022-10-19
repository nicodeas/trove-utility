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

void compress_file(char *filename)
{
    // Do we want this function else where as we are probably going to use this
    // In update/ remove functions as well?
    switch (fork())
    {
    case -1:
        // TODO
        // Change this later ...
        // Could we use errno/ perror?
        printf("Fork failed !!\n");
        exit(EXIT_FAILURE);
        break;
    case 0:
        // TODO
        // Check that this is successful
        execl("/usr/bin/gzip", "gzip", filename, NULL);
        //!!!
        // if trove file already exists, this command stops and prompts if you would like to override
        // is there a -y option to run this with so that this runs uninterupted?
        exit(EXIT_SUCCESS);
        break;

    default:
        wait(NULL);
        break;
    }
    printf("Compressed Successfully!\n");
}

void build_file(char *file_list[], char *filename, int file_count)
{
    FILE *fp = fopen(filename, "w");
    HASHTABLE *hashtable = hashtable_new();
    for (int i = 0; i < file_count; i++)
    {
        parse_fileargs(file_list[i], hashtable);
    }
    for (int i = 0; i < HASHTABLE_SIZE; i++)
    {
        while (hashtable[i] != NULL)
        {
            fprintf(fp, "#%s\n", hashtable[i]->word);
            LINK *links = hashtable[i]->link_to_paths;
            while (links != NULL)
            {
                fprintf(fp, "%s\n", links->path);
                links = links->next;
            }
            hashtable[i] = hashtable[i]->next;
        }
    }
    fclose(fp);
    printf("=====Parsing Complete=====\n");
    compress_file(filename);
}