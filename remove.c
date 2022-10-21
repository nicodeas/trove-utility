#include <stdio.h>
#include <stdlib.h>
#include "globals.h"
#include <string.h>

#include "remove.h"

void remove_path(HASHTABLE *hashtable, char *path)
{
  char basepath[PATH_MAX];
  realpath(path, basepath);
  int length = strlen(basepath);
  for (int i = 0; i < HASHTABLE_SIZE; i++)
  {
    HEAD_LINK *curr_head = hashtable[i];
    while (curr_head != NULL)
    {
      LINK *links = hashtable[i]->link_to_paths;

      while (links != NULL)
      {
        if ((links->path != NULL) && (strncmp(links->path, basepath, length) == 0))
        {
          free(links->path);
          links->path = NULL;
        }
        links = links->next;
      }
      curr_head = curr_head->next;
    }
  }
}

void remove_file(char *trovefile, char *paths[], int path_count)
{
  HASHTABLE *hashtable = read_trove_file(trovefile);
  for (int i = 0; i < path_count; i++)
  {
    remove_path(hashtable, paths[i]);
  }
  write_to_file(trovefile, hashtable);
}