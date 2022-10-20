#include <stdio.h>
#include <stdlib.h>
#include "globals.h"
#include <string.h>

#include "remove.h"

void remove_file(char *trovefile, char *paths[], int path_count)
{
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
  char *prev_string = NULL;
  bool prev_is_word = false;
  bool remove_path = false;
  FILE *fp = fopen("test.txt", "w");
  while (fgets(line, BUFSIZ, stream) != NULL)
  {
    free(prev_string);
    prev_string = strdup(line);
    if (*line == '#')
    {
      if (!prev_is_word)
      {
        fprintf(fp, "%s", prev_string);
      }
      prev_is_word = true;
    }
    else
    {
      for (int i = 0; i < path_count; i++)
      {
        int path_len = strlen(paths[i]);
        if (strncmp(paths[i], prev_string, path_len) == 0)
        {
          remove_path = true;
          break;
        }
      }
      if (!remove_path)
      {
        prev_is_word = false;
        fprintf(fp, "%s", prev_string);
      }
    }
  }

  close(fd[0]);
}