#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <dirent.h>
#include "node.h"
#include "library.h"

#define MP3_FILES_DIR_PATH "songs/"

char * concat(char *s1, char *s2) {
  char *cat_str = malloc(strlen(s1) + strlen(s2) + 1);
  strcpy(cat_str, s1);
  strcat(cat_str, s2);
  return cat_str;
}

/* returns true if file is an mp3 file */
int is_mp3(char * filename) {
  char * extension = strrchr(filename, '.');
  return (extension && strcmp(extension, ".mp3") == 0); // check if has extension and that extension is an mp3
}

/* Scans for mp3 files in given directory */
void scan_directory(char * path) {
  DIR * d = opendir(path);
  if (path == NULL) {
    perror("Error opening directory\n");
    return;
  }

  struct dirent *entry;
  while ((entry = readdir(d)) != NULL) {
    if (entry->d_type == DT_REG && is_mp3(entry->d_name)) {
      char * file_path = concat(path, entry->d_name);
      //...
      free(file_path); // free memory allocated by concat
    }
  }
}

void play_song() {
  getchar(); // clear newline character left by previous scanf
  char *song = malloc(256);
  printf("Enter song to play: ");
  scanf("%[^\n]", song);
  if (!is_mp3(song)) {
    song = concat(song, ".mp3");
  }
  char * song_path = concat(MP3_FILES_DIR_PATH, song);
  //need to get mp3 file associated w/ inputted song name
  //if no mp3 file --> MP3 file does not exist

  pid_t pid = fork();
  if (pid < 0) {
    perror("fork fail");
    exit(1);
  }
  if (pid == 0) {
    // prepare argument list for exec
    char *args[] = {"mpg123", song_path, NULL};

    // execute mpg123 program to play the MP3 file
    execvp("mpg123", args);
  }
  else { //parent
    printf("-\n");
    //printf("Playing %s...\n", song); //bug: prints even when file no exist
    printf("Press 'q' to quit and 'space' to pause\n");
    printf("-\n");
    wait(NULL);
  }

  free(song);

}

void add_song(struct song_node ** library) {
  //if lib size = max lib size --> say cant add anymore songs
  char input[256];
  printf("\nWould you like to manually add a song or automatically input one via MP3 file?\n");
  printf("-------\n");
  printf("1 - Manual\n");
  printf("2 - Automatic\n");
  printf("-------\n> ");

  scanf("%s", input);
  if (strcmp(input, "1") == 0) {
    getchar(); // clear newline character left by previous scanf
    char title[256];
    printf("Enter song title: ");
    scanf("%[^\n]", title);
    getchar();
    char artist[256];
    printf("Enter artist name: ");
    scanf("%[^\n]", artist);
    add(library, artist, title);
  }
  else if (strcmp(input, "2") == 0) {
    //auto
    // https://id3.org/
    // https://en.wikipedia.org/wiki/APE_tag
    scan_directory(MP3_FILES_DIR_PATH);

  }
  else {
    printf("\nInvalid command.\n");
    printf("Try: 1 or 2\n");
  }

  printf("\n");

  //LIB_SIZE++;
}

void remove_song(struct song_node ** library) {
  //if lib size = 0 --> say cant remove anymore songs
  getchar(); // clear newline character left by previous scanf
  char title[256];
  printf("Enter song title: ");
  scanf("%[^\n]", title);
  getchar();
  char artist[256];
  printf("Enter artist name: ");
  scanf("%[^\n]", artist);

  int has_song = delete_song(library, artist, title);
  if (has_song) {
    printf("\nRemoving: [%s: %s] \n", artist, title);
  }
  
  printf("\n");

  //LIB_SIZE++;
}

void randomize_songs(struct song_node ** library) {
  printf("\n");
  printf("Creating randomized playlist...\n");
  printf("Your Randomized Playlist:\n");
  shuffle(library); //kinda broken
  char input[256];
  printf("Would you like to save this playlist? (yes/no): ");
  scanf("%s", input);
  if (strcmp(input, "yes") == 0) {
    //save playlist
  }
}

// save library to file
void save_library(struct song_node ** library) {
  printf("\n");
  printf("Saving library...\n");
  int fd = open("library_save.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fd == -1) {
    perror("error opening file for saving library");
    return;
  }

  //for (int i = 0; i < LIB_SIZE; i++) {
  for (int i = 0; i < 27; i++) {
    if (library[i] != NULL) {
      dprintf(fd, "%c: ", i + 96);
      song_list_to_file(library[i], fd);
    }
  }

  close(fd);

  printf("Library saved!\n\n");
}
