#include "globals.h"
#include "trovelib.h"

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
      LINK *links = curr_head->link_to_paths;

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

HASHTABLE *remove_file(char *trovefile, char *paths[], int path_count)
{
  HASHTABLE *hashtable = read_trove_file(trovefile);
  for (int i = 0; i < path_count; i++)
  {
    remove_path(hashtable, paths[i]);
  }
  return hashtable;
}