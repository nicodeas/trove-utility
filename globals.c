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
        printf("\tCompressing %s\n", filename);
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

HASHTABLE *read_trove_file(char *filename)
{
    HASHTABLE *hashtable = hashtable_new();
    int fd[2];
    if (pipe(fd) == -1)
    {
        printf("Error creating pipe\n");
        exit(EXIT_FAILURE);
    };

    int pid = fork();
    if (pid == -1)
    {
        printf("Fork failed !!\n");
        exit(EXIT_FAILURE);
    }
    if (pid == 0)
    {
        dup2(fd[1], STDOUT_FILENO); // instead of writing to stdout, it writes to the pipe
        close(fd[0]);
        close(fd[1]);
        execl("/usr/bin/gzcat", "gzcat", filename, NULL);
    }
    close(fd[1]);
    FILE *stream;
    stream = fdopen(fd[0], "r");
    char line[BUFSIZ];
    HEAD_LINK *curr_head = NULL;
    while (fgets(line, BUFSIZ, stream) != NULL)
    {
        char *curr = line;
        line[strlen(line) - 1] = '\0';
        if (*line == '#')
        {
            curr_head = hashtable_add(hashtable, ++curr);
        }
        else
        {
            LINK *path_link = new_link(line);
            path_link->next = curr_head->link_to_paths;
            curr_head->link_to_paths = path_link;
        }
    }
    close(fd[0]);
    return hashtable;
}
