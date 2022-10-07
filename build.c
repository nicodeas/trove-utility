#include "trove.h"

void store_path(char *path, char *trovefile) {
  FILE *fp;
  fp = fopen(trovefile, "a");

  if(fp == NULL) {
    perror("fopen");
    exit(EXIT_FAILURE);
  }
  // Append path to file
  fprintf(fp, "%s\n", path);
  fclose(fp);
}

void get_path(char *filename, char *trovefile){
    char buf[PATH_MAX]; 
    char *res = realpath(filename, buf);
    if (res != NULL) { 
        printf("REAL PATH: %s \n", res);
        store_path(res, trovefile);
    } else {
        perror("realpath");
        exit(EXIT_FAILURE);
    }
}