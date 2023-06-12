#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE

#include <ctype.h>
#include <dirent.h>
#include <fcntl.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define DEFAULT_LENGTH 4
#define DEFAULT_TROVE_FILE_PATH "/tmp/trove"
#define HASHTABLE_SIZE 1572869
#define CHECK_ALLOC(p)                                                         \
  if (p == NULL) {                                                             \
    perror(__func__);                                                          \
    exit(EXIT_FAILURE);                                                        \
  }

extern int word_length;

typedef struct _link {
  char *path;
  struct _link *next;
} LINK;

typedef struct _head {
  char *word;
  struct _head *next;
  LINK *link_to_paths;

} HEAD_LINK;

typedef HEAD_LINK *HASHTABLE;

// HASHTABLE FUNCTIONS
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
extern HASHTABLE *read_trove_file(char *filename);

extern bool head_contains_path(HEAD_LINK *head);
extern void sanitise_hashtable(HASHTABLE *hashtable);

// TROVE
extern void usage(char *name, char error);

// BUILD
extern int unique_file_count;
extern int unique_words;
extern int word_count;
extern void process_word(char *fname, char *path, HASHTABLE *hashtable);
extern void parse_fileargs(char *file_arg, HASHTABLE *hashtable);
extern HASHTABLE *build_file(char *file_list[], int file_count,
                             HASHTABLE *hashtable);

// FIND
extern void find_word(char *trovefile, char *word);

// REMOVE
extern HASHTABLE *remove_file(char *trovefile, char *paths[], int path_count);
extern void remove_path(HASHTABLE *hashtable, char *path);
