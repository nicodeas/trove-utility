#include "trove.h"


int is_file(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

void read_file(char *file) {
  char * line = NULL;
  size_t len = 0;
  ssize_t read;
  FILE *fp    = fopen(file, "r");
  

//  REPORT AN ERROR, TERMINATE, IF FILE CANNOT BE OPENED
    if(fp == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    printf("===== READING FILE ======\n");
    while((read = getline(&line, &len, fp)) != -1) {
       printf("%s", line);
    }


}

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

void get_path(char *filename, char *trovefile, int length){
    char buf[PATH_MAX]; 
    char *res = realpath(filename, buf);
    if (res != NULL) { 
        printf("REAL PATH: %s \n", res);
        // Its a file
        if(is_file(res) == 1) {
          read_file(res);
          //store_path(res, trovefile);

        }
        // its a directory
        else {
          // tranverse directory here?

        }
    } else {
        perror("realpath");
        exit(EXIT_FAILURE);
    }
}