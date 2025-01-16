#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include "node.h"

#define LIB_SIZE 27 // library array has 27 indexes. index 0 is non alphabetical, index 1 thru 26 is A thru Z respectively. songs sorted by artists alphabetically in library

struct song_node ** init_song_lib() {
  struct song_node ** songArr = (struct song_node **)malloc(LIB_SIZE*sizeof(struct song_node));
  for (int i = 0; i < LIB_SIZE; i++) {
    songArr[i] = NULL;
  }
  return songArr;
}

int first_letter(char c) {
  if (!isalpha(c)) {
    return 0;
  }
  return toupper(c) - 64;
}

void add(struct song_node ** library, char * artist, char * title) {
  library[first_letter(artist[0])] = insert_song(library[first_letter(artist[0])],artist,title);
}

struct song_node * search_song(struct song_node ** library, char * artist, char * title ) {
  return find_song(library[first_letter(artist[0])],artist,title);
}

struct song_node * search_artist(struct song_node ** library, char * artist ) {
  return find_song_artist(library[first_letter(artist[0])],artist);
}

void print_letter(struct song_node ** library, char letter) {
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

void shuffle(struct song_node ** library) {
  srand(time(NULL));

  // go thru array of linked lists to create array of all songs in library in artist alphabetical order
  struct song_node * all_songs[1000];
  int count = 0; //song count
  for (int i = 0; i < LIB_SIZE; i++) {
    struct song_node * current = library[i];
    while (current != NULL) {
      all_songs[count++] = current;
      current = current->next;
    }
  }

  int fd = open("randomized_playlist_save.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fd == -1) {
    perror("Error opening file to save randomized playlist\n");
    return;
  }

  // until no songs left: getting random song, writing it to file & printing it, remove song from array
  int i = 1;
  while (count > 0) {
    int random_index = rand() % count;

    char line[300];
    sprintf(line, "%d. {%s, %s}\n", i, all_songs[random_index]->artist, all_songs[random_index]->title);
    printf("%s", line);
    write(fd, line, strlen(line));

    // remove used song and shift elements to left to fill gap
    for (int j = random_index; j < count-1; j++) {
      all_songs[j] = all_songs[j + 1];
    }

    count--;
    i++;

  }
  close(fd);
  printf("\n");
}

int delete_song(struct song_node ** library, char * artist, char * title ) {
  int song_found_staus = delete(&library[first_letter(artist[0])], artist, title);
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



void load_song_library(struct song_node ** library, FILE * file) {
    char artist[100], title[100];
    int len, len2;

    while (1) {
        fread(&len, sizeof(int), 1, file);
        printf("%d\n", len);

        size_t artist_len = fread(artist, sizeof(char), len, file);
        if (artist_len == 0) {
            break;
        }

        printf("%s\n", artist);

        fread(&len2, sizeof(int), 1, file);
        printf("%d\n", len2);

        size_t title_len = fread(title, sizeof(char), len2, file);

        printf("%ld, %ld\n", artist_len, title_len);
        //printf("%d, %d\n", len, len2);
        printf("%s, %s,\n", artist, title);
        add(library, artist, title);
    }
}

///maybe try concating and add a line break between artist and title

void save_song_library(struct song_node ** library, FILE * file) {
    for (int i = 0; i < LIB_SIZE; i++) {
        struct song_node * current = library[i];

        while (current != NULL) {
            printf("%s, %s,\n", current->artist, current->title);
            printf("%lu\n", strlen(current->artist));
            printf("%lu\n", strlen(current->title));
            int len = strlen(current->artist);
            int len2 = strlen(current->title);
            printf("%d, %d\n", len, len2);
            fwrite(&len, sizeof(int), 1, file);
            fwrite(current->artist, sizeof(char), strlen(current->artist)+1, file);
            //int len2 = strlen(current->title);
            fwrite(&len2, sizeof(int), 1, file);
            fwrite(current->title, sizeof(char), strlen(current->title)+1, file);
            current = current->next;
        }
    }
}
