#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct _path_link
{
    char *filepath;
    struct _path_link *next;
} PATH_LINK;

typedef struct _head_link
{
    char *word;
    struct _head_link *next;
    PATH_LINK *path_list;

} HEAD_LINK;

extern void store_path(char *path);
extern void get_path(char *filename);