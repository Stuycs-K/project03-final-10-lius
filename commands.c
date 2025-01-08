#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <dirent.h>
#include "library.h"

#define MP3_files_dir_path "./"

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

    }
  }
}

void play_song() {
  char song[256];
  printf("Enter song to play: ");
  if (fgets(song, sizeof(song), stdin) == NULL) {
    exit(1);
  }
  //need to get mp3 file associated w/ inputted song name
  //if no mp3 file --> MP3 file does not exist

  pid_t pid = fork();
  if (pid < 0) {
    perror("fork fail");
    exit(1);
  }
  if (pid == 0) {
    // prepare argument list for exec
    char *args[] = {"mpg123", song, NULL};

    // execute mpg123 program to play the MP3 file
    execvp("mpg123", args);
  }
  else { //parent
    printf("Playing song...\n");
    wait(NULL);
  }

}

void add_song(struct song_node ** library) {
  //if lib size = max lib size --> say cant add anymore songs
  char input[256];
  printf("Would you like to manually add a song or automatically input one via MP3 file?\n");
  printf("1 - Manual\n");
  printf("2 - Automatic\n");

  if (fgets(input, sizeof(input), stdin) == NULL) {
    //return;
    exit(1);
  }
  if (strcmp(input, "1") == 0) {
    char title[256];
    printf("Enter song title: ");
    if (fgets(input, sizeof(input), stdin) == NULL) { //double check later if this takes the who line w/ printed statement or just user input
      exit(1);
    }
    char artist[256];
    printf("Enter artist name: ");
    if (fgets(input, sizeof(input), stdin) == NULL) { //double check later if this takes the who line w/ printed statement or just user input
      exit(1);
    }
    add(library, artist, title);
  }
  else if (strcmp(input, "2") == 0) {
    //auto
    // https://id3.org/
    // https://en.wikipedia.org/wiki/APE_tag
    scan_directory(MP3_files_dir_path);

  }
  else {
    printf("Invalid command.\n");
    printf("Try: 1 or 2\n");
  }

  LIB_SIZE++;
}

void randomize_songs(struct song_node ** library) {
  printf("Creating randomized playlist...\n");
  printf("Your Randomized Playlist\n");
  shuffle(library);
  char input[256];
  printf("Would you like to save this playlist? (yes/no): ");
  if (fgets(input, sizeof(input), stdin) == NULL) {
    exit(1);
  }
  if (strcmp(input, "yes") == 0) {
    //save playlist
  }
}

// save library to file
void save_library(struct song_node ** library) {
  printf("Saving library...\n");
  int fd = open("library_save.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fd == -1) {
    perror("error opening file for saving library");
    return;
  }

  for (int i = 0; i < LIB_SIZE; i++) {
    if (library[i] != NULL) {
      dprintf(fd, "%c: ", i + 96);
      song_list_to_file(library[i], fd);
    }
  }

  close(fd);

  printf("Library saved!\n");
}
