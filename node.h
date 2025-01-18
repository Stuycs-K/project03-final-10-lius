#ifndef NODE_H
#define NODE_H

#define MAX_ARTIST_LEN 100
#define MAX_TITLE_LEN 100

struct song_node {char artist[MAX_ARTIST_LEN]; char title[MAX_TITLE_LEN]; struct song_node * next;};

int compare(struct song_node * a, struct song_node * b);
struct song_node * insert_song(struct song_node * list, char * artist, char * title);

void print_song_list(struct song_node * list);
void song_list_to_file(struct song_node * list, int fd);

struct song_node * find_song(struct song_node * list, char * artist, char * title);
struct song_node * find_song_artist(struct song_node * list, char * artist);

int delete(struct song_node ** list, char * artist, char * title);

struct song_node * free_list(struct song_node * list);

#endif