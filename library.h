#ifndef LIBRARY_H
#define LIBRARY_H

#define LIB_SIZE 27 // library array has 27 indexes. index 0 is non alphabetical, index 1 thru 26 is A thru Z respectively. songs sorted by artists alphabetically in library

struct song_node ** init_song_lib();

int first_letter(char c);

void add(struct song_node ** library, char * artist, char * title);
int delete_song(struct song_node ** library, char * artist, char * title);

struct song_node * search_song(struct song_node ** library, char * artist, char * title);
struct song_node * search_artist(struct song_node ** library, char * artist);

void print_artist(struct song_node ** library, char * artist);
void print_library(struct song_node ** library);

void shuffle(struct song_node ** library);

void reset(struct song_node ** library);

void load_song_library(struct song_node **library, FILE *file);
void save_song_library(struct song_node **library, FILE *file);

#endif