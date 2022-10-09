#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


int main(int argc, char *argv[]) {
  int fd[2]; 
  // fd[0] - read from
  // fd[1] - write to
  int p = pipe(fd);
  if(p == -1) {
    printf("Error: cannot open pipe\n");
  }
  int id = fork();
  switch (id)
  {
    case -1:
      printf("Error: cannot create process\n");
      break;
    case 0:
        close(fd[0]);
        int x = 10;
        printf("Child Process: %d \n", x);
        write(fd[1], &x, sizeof(x));
        close(fd[1]); 
        break;

    default:
        close(fd[1]);
        int y;
        read(fd[0], &y, sizeof(int));
        close(fd[0]);
        printf("Got from child process %d\n", y);
        break;
    }
  return 0;
}