#include "build.h"
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <limits.h>


void build() {
  printf("BUILDING NOW\n");
}
// void convert_to_alpha(char *line)
// {
//     char *ptr = line;
//     while (*ptr != '\0')
//     {
//         if (!isalpha(*ptr))
//         {
//             *ptr = ' ';
//         }
//         ++ptr;
//     }
// }

// int is_file(const char *path)
// {
//     struct stat path_stat;
//     stat(path, &path_stat);
//     return S_ISREG(path_stat.st_mode);
// }

// void read_file(char *file, int length) {
//   char * line = NULL;
//   size_t len = 0;
//   ssize_t read;
//   FILE *fp    = fopen(file, "r");
//   int count = 0;
  

// //  REPORT AN ERROR, TERMINATE, IF FILE CANNOT BE OPENED
//     if(fp == NULL) {
//         perror("fopen");
//         exit(EXIT_FAILURE);
//     }
//     printf("===== READING FILE ======\n");
//     while((read = getline(&line, &len, fp)) != -1) {
       
//        convert_to_alpha(line);
//       char *token = strtok(line, " ");
//       while (token != NULL)
//       {
//           if(strlen(token) >= length) {
//             printf("%s\n", token);
//             count++;
//           }
//           token = strtok(NULL, " ");
//       }
//     }
//     printf("====== File Stats =======\n");
//     printf("Words greater equal to %i: %i\n", length, count);


// }

// void store_path(char *path, char *trovefile) {
//   FILE *fp;
//   fp = fopen(trovefile, "a");

//   if(fp == NULL) {
//     perror("fopen");
//     exit(EXIT_FAILURE);
//   }
//   // Append path to file
//   fprintf(fp, "%s\n", path);
//   fclose(fp);
// }

// void get_path(char *filename, char *trovefile, int length){
//     char buf[PATH_MAX]; 
//     char *res = realpath(filename, buf);
//     if (res != NULL) { 
//         printf("REAL PATH: %s \n", res);
//         // Its a file
//         if(is_file(res) == 1) {
//           read_file(res, length);
//           //store_path(res, trovefile);

//         }
//         // its a directory
//         else {
//           // tranverse directory here?

//         }
//     } else {
//         perror("realpath");
//         exit(EXIT_FAILURE);
//     }
// }