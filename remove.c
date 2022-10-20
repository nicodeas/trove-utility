#include <stdio.h>
#include <stdlib.h>
#include "globals.h"
#include <string.h>
#include <libgen.h>

#include "remove.h"


// void process_line(char *line, char *path, HASHTABLE *hashtable)
// {

//     char delimter[2] = {" "};
//     char *token;
//     token = strtok(line, delimter);
//     while (token != NULL)
//     {
//         if (strlen(token) >= word_length)
//         {
//             printf("-- %s\n", token);
//             HEAD_LINK *head = hashtable_find(hashtable, token);
//             if (head == NULL)
//             {
//                 head = hashtable_add(hashtable, token);
//             }

//             LINK *ltp_cpy = head->link_to_paths;
//             if (!path_link_find(ltp_cpy, path))
//             {
//                 LINK *path_link = new_link(path);
//                 path_link->next = head->link_to_paths;
//                 head->link_to_paths = path_link;
                
//             }
            
//         }
//         token = strtok(NULL, delimter);
        
//     }
// }

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
  //char *prev_string = NULL;
  //bool prev_is_word = false;
  //bool remove_path = false;
  //FILE *fp = fopen("test.txt", "w");
  
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
              printf("%s\n", path_link->path);
          }
       
        }
      }
  }
  //   free(prev_string);
  //   prev_string = strdup(line);
  //   if (*line == '#')
  //   {
  //     if (!prev_is_word)
  //     {
  //       fprintf(fp, "%s", prev_string);
  //     }
  //     prev_is_word = true;
  //   }
  //   else
  //   {
  //     for (int i = 0; i < path_count; i++)
  //     {
  //       // char abs_path[PATH_MAX];
  //       // realpath(abs_path, paths[i]);
  //       // printf("PATH: %s\n", paths[i]);
  //       //  struct stat file_stat;
  //       // stat(abs_path, &file_stat);
       
  //       // if (S_ISDIR(file_stat.st_mode))
  //       // {
  //       //   printf("IM A DIR");


  //       // }
  //       // char *p = basename(prev_string);
  //       //printf("--- %s\n", p);
  //       // int len = strlen(p);
  //       // p[len -1] = '\0';
  //       // if (strcmp(paths[i], p) == 0)
  //       // {
  //       //   remove_path = true;
  //       //   break;
  //       // }
  //       printf("-- %s -- %s --", paths[i], prev_string);
  //       char *ret;
  //       int len = strlen(paths[i]);
        
  //       ret = strstr(prev_string, paths[i]);
  //       printf("^^ %i -- %c \n", len, ret[len]);
  //       ret[len + 1] = '\0';
  //       printf("-- %s -- %s -- %s\n", paths[i], prev_string, ret);
  //       // if (strcmp(ret, paths[i]) == 0)
  //       // {
  //       //   printf("TESTING");
  //       //   remove_path = true;
  //       //   break;
  //       // }
  //     }
  //     if (!remove_path)
  //     {
  //       prev_is_word = false;
  //       fprintf(fp, "%s", prev_string);
  //     }
  //   }
  // }

  

  close(fd[0]);
}