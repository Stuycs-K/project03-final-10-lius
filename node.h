#ifndef NODE_H
#define NODE_H

struct song_node {char artist[100]; char title[100]; struct song_node *next;};
struct song_node * insert_front(struct song_node* list, char* artist, char *title);
struct song_node * insert_song(struct song_node * list, char * artist, char * title);
void print_song_list(struct song_node * list);
void song_list_to_file(struct song_node * list, int fd);
int compare(struct song_node * a, struct song_node * b);
struct song_node * find_song(struct song_node * list, char * artist, char * title);
struct song_node * find_song_artist(struct song_node * list, char * artist);
struct song_node * random_song(struct song_node * list);
int delete_(struct song_node ** list, char * artist, char * title);
struct song_node * free_list(struct song_node * list);

#endif
