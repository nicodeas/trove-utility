#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

extern void store_path(char *path, char *trovefile);
extern void get_path(char *filename, char *trovefile, int length);
extern int is_file(const char *path);