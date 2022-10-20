#include <stdio.h>
#include <stdlib.h>
#include "globals.h"
#include <string.h>
#include <libgen.h>

#include "remove.h"


void rewrite_file(char *filename, HASHTABLE *hashtable)
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

void remove_file(char *trovefile, char *paths[], int path_count)
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
    execl("/usr/bin/gzcat", "gzcat", trovefile, NULL);
  }
  close(fd[1]);
  FILE *stream;
  stream = fdopen(fd[0], "r");
  char line[BUFSIZ];
  
  HEAD_LINK *head = NULL;
  while (fgets(line, BUFSIZ, stream) != NULL)
  {
      if(*line == '#') {
        memmove(line, line+1, strlen(line)); // Remove the hash
        head = hashtable_add(hashtable, line);
        printf("%s\n", head->word);
      }
      else {
        for (int i = 0; i < path_count; i++)
        {
          if(strstr(line, paths[i]) == NULL) {
              LINK *path_link = new_link(line);
              path_link->next = head->link_to_paths;
              head->link_to_paths = path_link;
          }
       
        }
      }
  }
  close(fd[0]);
  rewrite_file(trovefile, hashtable);
}