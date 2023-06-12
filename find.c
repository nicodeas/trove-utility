#include "globals.h"
#include "trovelib.h"

// Reads trove file into memory
void find_word(char *trovefile, char *word) {
  HASHTABLE *hashtable = read_trove_file(trovefile);
  HEAD_LINK *head = NULL;
  bool word_found = false;
  for (int i = 0; i < HASHTABLE_SIZE; i++) {
    while (hashtable[i] != NULL) {
      if (strcmp(hashtable[i]->word, word) == 0) {
        head = hashtable[i];
        word_found = true;
        break;
      }
      hashtable[i] = hashtable[i]->next;
    }
    if (word_found) {
      break;
    }
  }
  if (head != NULL) {
    LINK *links = head->link_to_paths;
    while (links != NULL) {
      printf("%s\n", links->path);
      links = links->next;
    }
  }
}
