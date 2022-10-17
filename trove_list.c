#include "trove_list.h"

HEAD_LINK *head_link_find(HEAD_LINK *head, char *wanted)
{
    HEAD_LINK *cpy = head;
    if (cpy != NULL)
    {
    }
    while (cpy != NULL)
    {
        if (strcmp(cpy->word, wanted) == 0)
        {
            return cpy;
        }
        cpy = cpy->next;
    }
    return NULL;
}

HEAD_LINK *new_head_link(char *string)
{
    HEAD_LINK *new = malloc(sizeof(HEAD_LINK));
    new->word = strdup(string);
    new->next = NULL;
    new->link_to_paths = NULL;
    return new;
}

LINK *new_link(char *path)
{
    LINK *new = malloc(sizeof(LINK));
    new->next = NULL;
    new->path = strdup(path);
    return new;
}

HEAD_LINK *head_link_add(HEAD_LINK *curr_head, char *string)
{
    HEAD_LINK *new = new_head_link(string);
    new->next = curr_head;
    return new;
}

bool path_link_find(LINK *link, char *wanted_path)
{
    while (link != NULL)
    {
        if (strcmp(link->path, wanted_path) == 0)
        {
            return true;
        }
        link = link->next;
    }
    return false;
    return true;
}

uint32_t hash_string(char *string)
{
    uint32_t hash = 0;

    while (*string != '\0')
    {
        hash = hash * 33 + *string;
        ++string;
    }
    return hash;
}

HASHTABLE *hashtable_new(void)
{
    HASHTABLE *new = calloc(HASHTABLE_SIZE, sizeof(HEAD_LINK *));

    return new;
}

HEAD_LINK *hashtable_add(HASHTABLE *hashtable, char *string)
{
    uint32_t h = hash_string(string) % HASHTABLE_SIZE; // choose list

    hashtable[h] = head_link_add(hashtable[h], string);
    return hashtable[h];
}

HEAD_LINK *hashtable_find(HASHTABLE *hashtable, char *string)
{
    char *cpy = string;
    uint32_t h = hash_string(cpy) % HASHTABLE_SIZE; // choose list
    return head_link_find(hashtable[h], string);
}

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
int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("%s: [wordlenght] [filename(s)] \n", argv[0]);
        exit(EXIT_FAILURE);
    }
    word_length = atoi(argv[1]);
    int output_fd = open("./trove-data.txt", O_WRONLY);
    int terminal_output_copy = dup(STDOUT_FILENO);

    switch (fork())
    {
    case 0:
        dup2(output_fd, STDOUT_FILENO);
        HASHTABLE *hashtable = hashtable_new();
        for (int i = 2; i < argc; i++)
        {
            parse_fileargs(argv[i], hashtable);
        }
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
        exit(EXIT_SUCCESS);
        break;
    default:
        wait(NULL);
        close(output_fd);
        dup2(terminal_output_copy, STDOUT_FILENO);
    }
    printf("parsing complete!\n");

    return 0;
}