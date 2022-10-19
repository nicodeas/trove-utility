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

extern void convert_to_alpha(char *line);
extern void process_line(char *line, char *path, HASHTABLE *hashtable);
extern void parse_file(char *fname, char *path, HASHTABLE *hashtable);
extern void parse_fileargs(char *file_arg, HASHTABLE *hashtable);

extern int build_file(char *file_list[], char *filename, int word_length, int file_count); // test function