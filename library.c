#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include "node.h"

//#define LIB_SIZE 27
//#define MAX_LIB_SIZE 27
const int MAX_LIB_SIZE = 27;
int LIB_SIZE;

struct song_node ** init(){
  struct song_node ** songArr = (struct song_node**)malloc(MAX_LIB_SIZE*sizeof(struct song_node));
  for(int i = 0; i < LIB_SIZE; i ++){
    songArr[i] = NULL;
  }
  return songArr;
}

int first_letter(char c){
  if(!isalpha(c)){
    return 0;
  }
  return toupper(c) - 64;
}

void add(struct song_node ** library, char * artist, char * title){
  library[first_letter(artist[0])] = insert_song(library[first_letter(artist[0])],artist,title);
}

struct song_node * search_song(struct song_node ** library, char * artist, char * title ){
  return find_song(library[first_letter(artist[0])],artist,title);
}

struct song_node * search_artist(struct song_node ** library, char * artist ){
  return find_song_artist(library[first_letter(artist[0])],artist);
}

void print_letter(struct song_node ** library, char letter){
  print_song_list(library[first_letter(letter)]);
}

void print_artist(struct song_node ** library, char * artist) {
  printf("Printing [%s] \n", artist);
  printf("%s: ", artist);
  struct song_node * current = search_artist(library, artist);
  if (current == NULL) {
      printf("\n\n");
      return;
  }
  printf("[ ");
  while (current->next != NULL && strcasecmp(current->next->artist, artist) == 0) {
      printf("{%s, %s} | ", current->artist, current->title);
      current = current->next;
  }
  printf("{%s, %s} ]\n\n", current->artist, current->title);
}

void print_library(struct song_node ** library) {
  for (int i = 0; i < LIB_SIZE; i++) {
    if (library[i] != NULL) {
      printf("%c: ", i + 96);
      print_song_list(library[i]);
    }
  }
  printf("\n");
}

void real_shuffle(struct song_node ** library, int n) {
  struct song_node * all_songs[1000];
  int count = 0;
  for (int i = 0; i < LIB_SIZE; i++) {
    struct song_node * current = library[i];
    while (current != NULL) {
      all_songs[count++] = current;
      current = current->next;
    }
  }
  if (count == 0) return; //library empty
  for (int i = 0; i < n; i++) {
    int random_index = rand() % count;
    printf("{%s, %s}\n", all_songs[random_index]->artist, all_songs[random_index]->title);
  }
  printf("\n");
}

int in_ary(int * ary, int size, int element) {
  for (int i = 0; i < size; i++) {
    if (element == ary[i]) {
      return 0;
    }
  }
  return 1;
}

void shuffle(struct song_node ** library) {
  struct song_node * all_songs[1000];
  int count = 0;
  for (int i = 0; i < LIB_SIZE; i++) {
    struct song_node * current = library[i];
    while (current != NULL) {
      all_songs[count++] = current;
      current = current->next;
    }
  }
  if (count == 0) return; //library empty
  int used_indexes[LIB_SIZE];
  for (int i = 0; i < LIB_SIZE; i++) {
    int random_index = rand() % count;
    if (in_ary(used_indexes, LIB_SIZE, random_index)) {
      char line[100];
      sprintf(line, "{%s, %s}\n", all_songs[random_index]->artist, all_songs[random_index]->title);
      printf("\n%s", line);
      int fd = open("randomized_playlist_save.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
      write(fd, line, sizeof(line));
    }
    i--;
  }
  printf("\n");
}

int delete_song(struct song_node ** library, char * artist, char * title ) {
  printf("removing: [%s: %s] \n", artist, title);
  int song_found_staus = remove_song(&library[first_letter(artist[0])], artist, title);
  print_library(library);
  return song_found_staus;
}

void reset(struct song_node ** library) {
  for (int i = 0; i < LIB_SIZE; i++) {
    if(library[i] != NULL){
      printf("freeing node: {%s, %s}\n",library[i]->artist,library[i]->title);
    }
    library[i] = free_list(library[i]);
  }
}
