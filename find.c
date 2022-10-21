#include "globals.h"
#include "trovelib.h"

// void find_word(char *trovefile, char *word)
// {
//     int fd[2];
//     if (pipe(fd) == -1)
//     {
//         printf("Error creating pipe\n");
//         exit(EXIT_FAILURE);
//     };

//     int pid = fork();
//     if (pid == -1)
//     {
//         printf("Fork failed !!\n");
//         exit(EXIT_FAILURE);
//     }
//     if (pid == 0)
//     {
//         dup2(fd[1], STDOUT_FILENO); // instead of writing to stdout, it writes to the pipe
//         close(fd[0]);
//         close(fd[1]);
//         execl("/usr/bin/gzcat", "gzcat", "-q", trovefile, NULL);
//     }
//     close(fd[1]);
//     FILE *stream;
//     stream = fdopen(fd[0], "r");
//     char line[BUFSIZ];
//     bool word_found = false;

//     while (fgets(line, BUFSIZ, stream) != NULL)
//     {
//         char *curr = line;
//         line[strlen(line) - 1] = '\0';
//         curr++;
//         if (*line == '#')
//         {
//             if (word_found)
//             {
//                 break;
//             }
//             if (strcmp(word, curr) == 0)
//             {
//                 word_found = true;
//                 continue;
//             }
//         }
//         if (word_found)
//         {
//             printf("%s\n", line);
//         }
//     }
//     close(fd[0]);
// }

// Reads trove file into memory
void find_word(char *trovefile, char *word)
{
    HASHTABLE *hashtable = read_trove_file(trovefile);
    HEAD_LINK *head = NULL;
    bool word_found = false;
    for (int i = 0; i < HASHTABLE_SIZE; i++)
    {
        while (hashtable[i] != NULL)
        {
            if (strcmp(hashtable[i]->word, word) == 0)
            {
                head = hashtable[i];
                word_found = true;
                break;
            }
            hashtable[i] = hashtable[i]->next;
        }
        if (word_found)
        {
            break;
        }
    }
    if (head != NULL)
    {
        LINK *links = head->link_to_paths;
        while (links != NULL)
        {
            printf("%s\n", links->path);
            links = links->next;
        }
    }
}
