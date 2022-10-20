#include <stdio.h>
#include <stdlib.h>
#include "globals.h"
#include <string.h>
#include <libgen.h>

#include "remove.h"


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
  write_to_file(trovefile, hashtable);
}