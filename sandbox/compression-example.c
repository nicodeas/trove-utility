#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <string.h>

#include <getopt.h>
#define OPTLIST "c:u:" // c: compress file, u: uncompress file

void usage(char *name, char error)
{
    switch (error)
    {
    case '?':
        printf("Usage: %s [-c | -u] filename \n", name);
        break;
    }
    exit(EXIT_FAILURE);
}

void compress_file(char *file) {
  printf("======= Compressing %s =======\n", file);
  int pid = fork();
  switch(pid) {
    case -1:
      exit(EXIT_FAILURE);
      break;
    case 0: 
      // Child process
      // execl("/usr/bin/gzip","/usr/bin/gzip", file, NULL);
      // execlp("gzip", "gzip", "-k", file, NULL); //  keep the orig file
      execlp("gzip", "gzip", file, NULL); // Dont need to include the full path - gets from $PATH vars
      
      printf("This shouldn't print\n");
      break;
    case 1:
      // Parent process
      wait(NULL);
      printf("Successful!\n");
      break;
  }

}

void uncompress_file(char *file) {
  file = strcat(file, ".gz");
  printf("======= Uncompressing %s =======\n", file);
  int pid = fork();
  switch(pid) {
    case -1:
      exit(EXIT_FAILURE);
      break;
    case 0: 
      // Child process
      // execl("/usr/bin/gzip","/usr/bin/gzip", "-d", file, NULL);
      execlp("gzip", "gzip", "-d", file, NULL); // Don't need to include the full path
      
      printf("This shouldn't print\n");
      break;
    case 1:
      // Parent process
      wait(NULL);
      printf("Successful!\n");
      break;
  }

}

int main(int argc, char *argv[]) {

  bool cflag = false;
  bool uflag = false;
  int opt;
  char *file = NULL;

    while ((opt = getopt(argc, argv, OPTLIST)) != -1)
    {
        printf("Option = %c\n", opt);
        switch (opt) {
          case 'c':
            cflag = true;
            file = optarg;
            break;
          case 'u':
            uflag = true;
            file = optarg;
            break;
          case '?':
            usage(argv[0], '?');
            break;
        }
    }


    if (cflag) {
      compress_file(file);

    }
    if(uflag) {
      uncompress_file(file);
    }
}