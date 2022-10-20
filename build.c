#include "globals.h"
#include "build.h"

int word_length;
int unique_file_count;
int unique_words;
int word_count;

void convert_to_alpha(char *line)
{
    char *ptr = line;
    while (*ptr != '\0')
    {
        if (!isalnum(*ptr))
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
            ++word_count;
            HEAD_LINK *head = hashtable_find(hashtable, token);
            if (head == NULL)
            {
                head = hashtable_add(hashtable, token);
               ++unique_words;
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
    printf("\tSearching words in '%s'\n", path);
    FILE *fp = fopen(fname, "r");
    char line[BUFSIZ];
    unique_file_count++;
    if (fp == NULL)
    {
        perror(fname);
        exit(EXIT_FAILURE);
    }
    while (fgets(line, BUFSIZ, fp) != NULL)
    {
        printf("READING LINE:   %s  \n", line);
        convert_to_alpha(line);
        process_line(line, path, hashtable);
    }
    printf("\tfound %i words\n", word_count);
    word_count = 0;
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
    int pipe_one[2]; // main write, gzip read
    int pipe_two[2]; // gzip write , main read
    int terminal_output_copy = dup(STDOUT_FILENO);
    if ((pipe(pipe_one) == -1) || (pipe(pipe_two) == -1))
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
        dup2(pipe_one[0], STDIN_FILENO);
        dup2(pipe_two[1], STDOUT_FILENO);
        close(pipe_one[0]);
        close(pipe_one[1]);
        close(pipe_two[0]);
        close(pipe_two[1]);

        if (execl("/usr/bin/gzip", "gzip", NULL) == -1)
        {
            perror("error");
        }
        break;

    default:
        close(pipe_one[0]);
        close(pipe_two[1]);
        dup2(pipe_one[1], STDOUT_FILENO);
        for (int i = 0; i < HASHTABLE_SIZE; i++)
        {
            while (hashtable[i] != NULL)
            {
                printf("#%s\n", hashtable[i]->word);
                // this does not work as well unsure why the data piped is not in the correct form
                // write(pipe_one[1], hashtable[i]->word, sizeof(hashtable[i]->word));
                LINK *links = hashtable[i]->link_to_paths;
                while (links != NULL)
                {
                    printf("%s\n", links->path);
                    // this does not work as well unsure why the data piped is not in the correct form
                    // write(pipe_one[1], links->path, sizeof(links->path));
                    links = links->next;
                }
                hashtable[i] = hashtable[i]->next;
            }
        }
        dup2(terminal_output_copy, STDOUT_FILENO);
        close(pipe_one[1]);
        FILE *output_file = fopen(filename, "w");
        FILE *compressed = fdopen(pipe_two[0], "r");
        char c;
        while (!feof(compressed))
        {
            c = fgetc(compressed);
            fputc(c, output_file);
        }

        // int out = open(filename, O_WRONLY | O_CREAT);
        // char line[BUFSIZ];
        // if
        // while (read(pipe_two[0], line, BUFSIZ) > 0)
        // {
        //     // write(out, line, sizeof(line));
        //     fprintf(output_file, "%s", line);
        // }
        close(pipe_two[0]);
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
    write_to_file(filename, hashtable);
    printf("=====Parsing Complete!=====\n");
    printf("\t%i unique files\n", unique_file_count);
    printf("\t%i unique words\n", unique_words);
}
