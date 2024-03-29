#include "trove.h"

int word_length;
int unique_file_count;
int unique_words;
int word_count;

void process_word(char *word, char *path, HASHTABLE *hashtable) {
  int length = strlen(word);
  if (length >= word_length) {
    ++word_count;
    HEAD_LINK *head = hashtable_find(hashtable, word);

    if (head == NULL) {
      head = hashtable_add(hashtable, word);
      ++unique_words;
    }

    LINK *ltp_cpy = head->link_to_paths;
    if (!path_link_find(ltp_cpy, path)) {
      LINK *path_link = new_link(path);
      path_link->next = head->link_to_paths;
      head->link_to_paths = path_link;
    }
  }
}

void parse_file(char *fname, char *path, HASHTABLE *hashtable) {
  FILE *fp = fopen(fname, "r");
  if (fp == NULL) {
    perror(fname);
    exit(EXIT_FAILURE);
  }
  printf("\tSearching words in '%s'\n", path);
  unique_file_count++;

  int c;
  char word[BUFSIZ];
  char *window;
  window = word;

  while ((c = fgetc(fp)) != EOF) {
    if (isalnum(c)) {
      *window = c;
      window++;
    } else {
      *window = '\0';
      process_word(word, path, hashtable);
      window = word;
    }
  }
  fclose(fp);
  printf("\tFound %i words\n", word_count);
  word_count = 0;
}

void parse_fileargs(char *file_arg, HASHTABLE *hashtable) {
  char base_path[PATH_MAX];
  char path[PATH_MAX];
  struct stat file_stat;
  realpath(file_arg, base_path);
  stat(base_path, &file_stat);
  if (S_ISDIR(file_stat.st_mode)) {
    DIR *dirp;
    struct dirent *dp;
    dirp = opendir(file_arg);
    if (dirp == NULL) {
      perror(file_arg);
      exit(EXIT_FAILURE);
    }
    while ((dp = readdir(dirp)) != NULL) {

      if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..")) {

        strcpy(path, base_path);
        strcat(path, "/");
        strcat(path, dp->d_name);
        parse_fileargs(path, hashtable);
      }
    }
    closedir(dirp);
  } else {
    parse_file(file_arg, base_path, hashtable);
  }
}

HASHTABLE *build_file(char *file_list[], int file_count, HASHTABLE *hashtable) {
  if (hashtable == NULL) {
    hashtable = hashtable_new();
  }
  printf("===== Parsing Files =====\n");
  for (int i = 0; i < file_count; i++) {
    parse_fileargs(file_list[i], hashtable);
  }
  printf("===== Parsing Complete =====\n");
  printf("\t%i unique files\n", unique_file_count);
  printf("\t%i unique words\n", unique_words);
  return hashtable;
}
