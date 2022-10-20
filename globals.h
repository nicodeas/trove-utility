#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/wait.h>

#define DEFAULT_LENGTH 4
#define DEFAULT_TROVE_FILE_PATH "/tmp/trove"
#define HASHTABLE_SIZE 1572869
extern int word_length;

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

typedef HEAD_LINK *HASHTABLE;

extern HEAD_LINK *head_link_find(HEAD_LINK *head, char *wanted);
extern HEAD_LINK *new_head_link(char *string);
extern LINK *new_link(char *path);
extern HEAD_LINK *head_link_add(HEAD_LINK *curr_head, char *string);

extern bool path_link_find(LINK *link, char *wanted_path);
extern uint32_t hash_string(char *string);

extern HASHTABLE *hashtable_new(void);
extern HEAD_LINK *hashtable_add(HASHTABLE *hashtable, char *string);
extern HEAD_LINK *hashtable_find(HASHTABLE *hashtable, char *string);

extern void write_to_file(char *filename, HASHTABLE *hashtable);