#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <string.h>

#include <getopt.h>
#define OPTLIST "c:u:r:" // c: compress file, u: uncompress file, r: read uncompressed file


#define WRITE 1
#define READ 0


void usage(char *name, char error)
{
    switch (error)
    {
    case '?':
        printf("Usage: %s [-c | -u | -r] filename \n", name);
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


void read_compressed_file(char *file) {
  printf("Reading compressed file\n");
  int fd[2];
  pid_t pid;
  char buf[4096];

  // int redirect_fd = open("redirected", O_CREAT | O_TRUNC | O_WRONLY);
  // dup2(redirect_fd, STDOUT_FILENO);
  // execlp("gzcat", "gzcat", file, NULL);
  // close(redirect_fd);
  if (pipe(fd)==-1) {
    exit(EXIT_FAILURE);
  }
  if ((pid = fork()) == -1) {
    exit(EXIT_FAILURE);
  }

  if(pid == 0) {

    dup2 (fd[1], STDOUT_FILENO);
    close(fd[0]);
    close(fd[1]);
    execlp("gzcat", "gzcat", file, (char *)0);
    exit(EXIT_FAILURE);

  } else {
    close(fd[1]);
    ssize_t nbytes;
    while((nbytes = read(fd[0], buf, sizeof(buf))) >0) {
      printf("%s\n", buf);
    }
    wait(NULL);

  }

}

int main(int argc, char *argv[]) {

  bool cflag = false;
  bool uflag = false;
  bool rflag = false;
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
          case 'r':
            rflag = true;
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
    else if(uflag) {
      uncompress_file(file);
    }
    else if(rflag) {
      read_compressed_file(file);
    }
}