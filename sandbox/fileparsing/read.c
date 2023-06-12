#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>
#include <limits.h> /* PATH_MAX */
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

typedef struct _link
{
    char *path;
    struct _link *next;
} LINK;

typedef struct _head
{
    char *word;
    struct _head *next;
    LINK *link_to_paths;

} HEAD_LINK;

HEAD_LINK *trove;

HEAD_LINK *find_word(char *word)
{
    HEAD_LINK *head = trove;
    while (head != NULL)
    {
        if (strcmp(head->word, word) == 0)
        {
            return head;
        }
        head = head->next;
    }
    return NULL;
}

HEAD_LINK *init_head(char *word)
{
    // add to front of list
    HEAD_LINK *new_head = malloc(sizeof(HEAD_LINK));
    new_head->word = strdup(word);
    new_head->next = trove;
    new_head->link_to_paths = NULL;
    trove = new_head;
    return trove;
}
LINK *init_link(char *path)
{
    LINK *new_link = malloc(sizeof(LINK));
    new_link->path = strdup(path);
    new_link->next = NULL;
    return new_link;
}



void read_file(char *file) {
  char * line = NULL;
  size_t len = 0;
  ssize_t read;
  FILE *fp    = fopen(file, "r");
  

//  REPORT AN ERROR, TERMINATE, IF FILE CANNOT BE OPENED
    if(fp == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    printf("===== READING FILE ======\n");
    while((read = getline(&line, &len, fp)) != -1) {
      char *token = strtok(line, " | ");
      while (token != NULL)
      {
            printf("%s\n", token);
          
          token = strtok(NULL, " | ");
      }
    }
}




int main(int argc, char *argv[]) {

  read_file("trove-file");
}