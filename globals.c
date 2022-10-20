#include "globals.h"

HEAD_LINK *head_link_find(HEAD_LINK *head, char *wanted)
{
    while (head != NULL)
    {
        if (strcmp(head->word, wanted) == 0)
        {
            return head;
        }
        head = head->next;
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

void write_to_file(char *filename, HASHTABLE *hashtable)
{
    printf("\tWriting %s\n", filename);
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
        printf("\tCompressing %s\n", filename);
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
