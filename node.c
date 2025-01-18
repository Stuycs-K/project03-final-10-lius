#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "node.h"

/* Compares 2 song_nodes case-insensitive by ASCII.
 * If artists are the same, compare titles.
*/
int compare(struct song_node * a, struct song_node * b){
  int c = strcasecmp(a->artist, b->artist);
  if (c == 0) {
    return strcasecmp(a->title, b->title);
  }
  return c;
}

/* Inserts new song to given linked list.
 * Creates new song using given artist and title.
 * Compares alphabetically new song against songs in list. Inserts accordingly.
*/
struct song_node * insert_song(struct song_node * list, char * artist, char * title){
  struct song_node * new_song = (struct song_node *)malloc(sizeof(struct song_node));
  strcpy(new_song->title, title);
  strcpy(new_song->artist, artist);
  struct song_node * p = list;
  struct song_node * prev = NULL;
  if (list == NULL || compare(list, new_song) > 0) {
    new_song->next = list;
    return new_song;
  }
  while (!(list == NULL || compare(list, new_song) > 0)) {
    prev = list;
    list = list->next;
  }
  prev->next = new_song;
  new_song->next = list;
  return p;
}

/* Print given linked list */
void print_song_list(struct song_node * list){
  if (list == NULL) {
    printf("[ ]\n");
    return;
  }
  printf("[ ");
  while (list->next != NULL) {
    printf("{%s, %s} | ", list->artist, list->title);
    list = list->next;
  }
  printf("{%s, %s} ]\n", list->artist, list->title);
}

/* Write linked list to file */
void song_list_to_file(struct song_node * list, int fd){
  if (list == NULL) {
    dprintf(fd, "[ ]\n");
    return;
  }
  dprintf(fd, "[ ");
  while (list->next != NULL) {
    dprintf(fd, "{%s, %s} | ", list->artist, list->title);
    list = list->next;
  }
  dprintf(fd, "{%s, %s} ]\n", list->artist, list->title);
}

/* Find song associated with given artist and title in given linked list.
 * If found, returns pointer to node.
 * If not found, returns NULL.
*/
struct song_node * find_song(struct song_node * list, char * artist, char * title){
  while(list != NULL){
    if(strcmp(list->artist, artist) == 0 && strcmp(list->title, title) == 0){
      return list;
    }
    list = list->next;
  }
  return NULL;
}

/* Find song associated with given artist .
 * If found, returns pointer to first song w/ the artist.
 * If no songs found, returns NULL.
*/
struct song_node * find_song_artist(struct song_node * list, char * artist) {
  while (list != NULL) {
    if (strcasecmp(list->artist, artist) == 0) {
      return list;
    }
    list = list->next;
  }
  return NULL;
}

/* Remove song associated with given artist and title from list.
 * Returns 1 if song found.
 * Returns 0 if song not found.
*/
int delete(struct song_node ** list, char * artist, char * title) {
  struct song_node * prev_node = NULL;
  struct song_node * current = *list;
  while (current != NULL) {
    if ((strcmp(current->artist, artist) == 0) && (strcmp(current->title, title) == 0)) {
      if (prev_node != NULL) {
        prev_node->next = current->next;
      }
      else {
        *list = current->next;
      }
      free(current);
      return 1;
    }
    prev_node = current;
    current = current->next;
  }
  return 0;
}

/* Free given linked list */
struct song_node * free_list(struct song_node * list){
  while (list != NULL) {
    free(list);
    list = list->next;
  }
  return NULL;
}