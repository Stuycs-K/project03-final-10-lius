#ifndef LIBRARY_H
#define LIBRARY_H

struct song_node ** init_song_lib(); 
int first_letter(char c);
void add(struct song_node ** library, char * artist, char * title);
struct song_node * search_song(struct song_node ** library, char * artist, char * title);
struct song_node * search_artist(struct song_node ** library, char * artist);
void print_letter(struct song_node ** library, char letter);
void print_artist(struct song_node ** library, char * artist);
void print_library(struct song_node ** library);
void real_shuffle(struct song_node ** library, int n);
void shuffle(struct song_node ** library);
int delete_song(struct song_node ** library, char * artist, char * title);
int reset(struct song_node ** library);

#endif
