#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include "node.h"
#include "commands.h"

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
  struct song_node * all_songs[get_song_count()];
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


/* returns number of songs loaded in */
void load_song_library(struct song_node ** library, FILE * file) {
  char artist[100], title[100];
  int artist_len, title_len;

  while (1) {
    int ch = fgetc(file);
    if (ch == '\n') {
      break; // end of current struct
    }
    ungetc(ch, file); // push char back into input stream


    fread(&artist_len, sizeof(int), 1, file);
    size_t byte_read = fread(artist, sizeof(char), artist_len+1, file);
    if (byte_read == 0) {
      break;  // end of songs
    }

    fread(&title_len, sizeof(int), 1, file);
    fread(title, sizeof(char), title_len+1, file);

    add(library, artist, title);
    inc_song_count_by_1();
  }
}

void save_song_library(struct song_node ** library, FILE * file) {
  // iterate over each letter index (A-Z)
  for (int i = 0; i < LIB_SIZE; i++) {
  
    struct song_node * current = library[i];

    while (current != NULL) {
      // remove newline character if present
      current->title[strcspn(current->title, "\n")] = '\0';
      current->artist[strcspn(current->artist, "\n")] = '\0';
      
      // write artist length, artist, title length, and title to file
      int artist_len = strlen(current->artist);
      int title_len = strlen(current->title);

      fwrite(&artist_len, sizeof(int), 1, file);
      fwrite(current->artist, sizeof(char), artist_len+1, file);

      fwrite(&title_len, sizeof(int), 1, file);
      fwrite(current->title, sizeof(char), title_len+1, file);

      current = current->next;
    }
  }
}
