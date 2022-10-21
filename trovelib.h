// ---- TROVE
extern void usage(char *name, char error);

// ---- BUILD
// extern void process_word(char *line, char *path, HASHTABLE *hashtable);
// extern void parse_file(char *fname, char *path, HASHTABLE *hashtable);
extern int unique_file_count;
extern int unique_words;
extern int word_count;
extern void process_word(char *fname, char *path, HASHTABLE *hashtable);
extern void parse_fileargs(char *file_arg, HASHTABLE *hashtable);
extern HASHTABLE *build_file(char *file_list[], int file_count, HASHTABLE *hashtable); // test function


// ---- FIND
extern void find_word(char *trovefile, char *word);


// ---- REMOVE
extern HASHTABLE *remove_file(char *trovefile, char *paths[], int path_count);


// ---- UPDATE
extern void update_file();