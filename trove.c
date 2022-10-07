#include "trove.h"

int main(int argc, char *argv[]) {
  // Handle options here

  // Currently it gets the filenames from the command line arg
  // e.g. ./trove trove.h Makefile build.o build.c
  for(int i = 1; i < argc; i++) {
    get_path(argv[i]);
  }
}