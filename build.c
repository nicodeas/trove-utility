#include "trove.h"

void store_path(char *path) {
  FILE *fp;
  // TODO: Make sure trove-file is not hard coded.
  fp = fopen("trove-file", "a");

  if(fp == NULL) {
    //perror(path);
    exit(EXIT_FAILURE);
  }
  // Append path to file
  fprintf(fp, "%s\n", path);
  fclose(fp);
}

void get_path(char *filename){
    char buf[PATH_MAX]; 
    char *res = realpath(filename, buf);
    if (res != NULL) { 
        printf("REAL PATH: %s \n", buf);
        store_path(buf);
    } else {
        perror("realpath");
        exit(EXIT_FAILURE);
    }
}