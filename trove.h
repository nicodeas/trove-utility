#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

// ---------------------------------------
// | Cat | PATH1 | PATH2 | PATH3 | PATH4 |
// ---------------------------------------
// -------------------------------------------------------
// | Dog | PATH1 | PATH2 | PATH3 | PATH4 | PATH5 | PATH6 |
// -------------------------------------------------------
// -------------------------
// | Apple | PATH1 | PATH2 |
// -------------------------
// ----------------
// | Pear | PATH1 |
// ----------------

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

extern PATH_LINK new_path_link();
extern bool path_exists(HEAD_LINK *head, char *path);

extern HEAD_LINK new_head_link();
extern HEAD_LINK find_head_link(HEAD_LINK *head, char *word);
extern void free_head_link(HEAD_LINK *head);