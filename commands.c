#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "library.h"

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
}
