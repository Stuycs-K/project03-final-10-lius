#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "node.h"

struct song_node * insert_front(struct song_node * list, char * artist, char * title){
  struct song_node * newSong = (struct song_node *)malloc(sizeof(struct song_node));
  strcpy(newSong->title, title);
  strcpy(newSong->artist, artist);
  newSong->next = list;
  return newSong;
}

int compare(struct song_node * a, struct song_node * b){
  int c = strcasecmp(a->artist, b->artist);
  if (c == 0) {
    return strcasecmp(a->title, b->title);
  }
  return c;
}

struct song_node * insert_song(struct song_node * list, char * artist, char * title){
  struct song_node * newSong = (struct song_node *)malloc(sizeof(struct song_node));
  strcpy(newSong->title, title);
  strcpy(newSong->artist, artist);
  struct song_node * p = list;
  struct song_node * prev = NULL;
  if (list == NULL || compare(list, newSong) > 0) {
    newSong->next = list;
    return newSong;
  }
  while (!(list == NULL || compare(list, newSong) > 0)) {
    prev = list;
    list = list->next;
  }
  prev->next = newSong;
  newSong->next = list;
  return p;
}

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

struct song_node * find_song(struct song_node * list, char * artist, char * title){
  while(list != NULL){
    if(strcmp(list->artist, artist) == 0 && strcmp(list->title, title) == 0){
      return list;
    }
    list = list->next;
  }
  return NULL;
}

struct song_node * find_song_artist(struct song_node * list, char * artist) {
  while (list != NULL) {
    if (strcasecmp(list->artist, artist) == 0) {
      return list;
    }
    list = list->next;
  }
  return NULL;
}

struct song_node * random_song(struct song_node * list) {
  if (list == NULL) return NULL;

  int count = 0;
  struct song_node * current = list;
  while (current != NULL) {
    count++;
    current = current->next;
  }
  srand(time(NULL));
  int index = rand() % count;
  while (index) {
    index--;
    list = list->next;
  }
  return list;
}

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

struct song_node * free_list(struct song_node * list){
  while (list != NULL) {
    free(list);
    list = list->next;
  }
  return NULL;
}
