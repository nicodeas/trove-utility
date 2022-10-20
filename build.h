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

// extern void process_word(char *line, char *path, HASHTABLE *hashtable);
// extern void parse_file(char *fname, char *path, HASHTABLE *hashtable);
extern void process_word(char *fname, char *path, HASHTABLE *hashtable);
extern void parse_fileargs(char *file_arg, HASHTABLE *hashtable);

extern void build_file(char *file_list[], char *filename, int file_count); // test function