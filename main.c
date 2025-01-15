#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include "node.h"
#include "library.h"
#include "commands.h" // song related commands

int main() {

  struct song_node ** library = init_song_lib();
  struct user ** account_lib = init_acct_lib();
  //load_accounts(account_lib);

  char input[256];

  while (1) {
    printf("=====================\n");
    printf("Your current library:\n");
    //printf("--- Your current library ---\n");
    print_library(library);

    printf("-------\n");
    printf("What would you like to do? (enter corresponding number/letter)\n");
    printf("-------\n");
    printf("1 - Play song\n");
    printf("2 - Add song\n");
    printf("3 - Remove song\n");
    printf("4 - Create randomized playlist\n");
    printf("5 - Save library\n");
    printf("6 - Clear library\n");
    printf("q - Quit\n");
    printf("-------\n> ");
    //printf("x - Delete account\n");

    scanf("%s", input);
    if (strcmp(input, "1") == 0) {
      play_song();
    }
    else if (strcmp(input, "2") == 0) {
      add_song(library);
    }
    else if (strcmp(input, "3") == 0) {
      remove_song(library);
    }
    else if (strcmp(input, "4") == 0) {
      randomize_songs(library);
    }
    else if (strcmp(input, "5") == 0) {
      save_library(library);
    }
    else if (strcmp(input, "6") == 0) { //added ability to clear library
      printf("Clearing library...\n");
      reset(library);
      printf("Library cleared!\n\n");
    }
    else if (strcmp(input, "7") == 0) {
      login(account_lib);
    }
    else if (strcmp(input, "8") == 0) {
      create_account(account_lib);
    }
    else if (strcmp(input, "9") == 0) {
      save_accounts(account_lib);
    }
    else if (strcmp(input, "q") == 0) {
      return 0;
    }

  }

  return 0;

}





















//   srand(time(NULL));
//   //Linked List Tests
//   printf("LINKED LIST TESTS\n");
//   struct song_node* songList = NULL;
//   printf("========================================\n");
//   printf("TESTING INSERT FRONT:\n");
//   songList = insert_front(songList, "Taylor Swift", "Shake it Off");
//   songList = insert_front(songList, "Adele", "Hello");
//   print_song_list(songList);
//   printf("========================================\n");
//   printf("TESTING INSERT SONG:\n");
//   songList = insert_song(songList, "Imagine Dragons", "Believer");
//   songList = insert_song(songList, "Alan Walker", "Spectre");
//   songList = insert_song(songList, "Post Malone", "Sunflower");
//   songList = insert_song(songList, "Imagine Dragons", "Radioactive");
//   songList = insert_song(songList, "Alan Walker", "Faded");
//   print_song_list(songList);
//   printf("========================================\n");
//   printf("TESTING FIND SONG:\n");
//   struct song_node *found_song = find_song(songList, "Imagine Dragons", "Believer");
//   printf("Looking for Believer by Imagine Dragons: %s, %s\n", found_song->artist, found_song->title);
//   printf("========================================\n");
//   printf("TESTING FIND ARTIST:\n");
//   found_song = find_song_artist(songList, "Alan Walker");
//   printf("Looking for Alan Walker:\n");
//   print_song_list(found_song);
//   printf("========================================\n");
//   printf("TESTING RANDOM:\n");
//   found_song = random_song(songList);
//   printf("{%s, %s}\n",found_song->artist,found_song->title);
//   found_song = random_song(songList);
//   printf("{%s, %s}\n",found_song->artist,found_song->title);
//   found_song = random_song(songList);
//   printf("{%s, %s}\n",found_song->artist,found_song->title);
//   printf("========================================\n");
//   printf("TESTING REMOVE:\n");
//   printf("Removing [Imagine Dragons: Radioactive]\n");
//   remove_song(&songList,"Imagine Dragons", "Radioactive");
//   print_song_list(songList);
//   printf("Removing [Adele: Hello]\n");
//   remove_song(&songList,"Adele", "Hello");
//   print_song_list(songList);
//   printf("Removing [Alan Walker: Faded]\n");
//   remove_song(&songList,"Alan Walker", "Faded");
//   print_song_list(songList);
//   printf("========================================\n");
//   printf("FREEING SONGS:\n");
//   songList = free_list(songList);
//   print_song_list(songList);
//   printf("========================================\n");

//   //Library tests
//   printf("\n LIBRARY TESTS \n");
//   struct song_node ** library = init();
//   printf("========================================\n");
//   printf("TESTING PRINT LETTER\n");
//   print_letter(library,'a');
//   add(library, "Adele", "Hello");
//   add(library, "Taylor Swift", "Shake it Off");
//   add(library, "Imagine Dragons", "Believer");
//   add(library, "Post Malone", "Sunflower");
//   add(library, "Imagine Dragons", "Radioactive");
//   add(library, "Alan Walker", "Faded");
//   add(library, "Alan Walker", "Spectre");
//   printf("PRINTING LETTER A \n");
//   print_letter(library,'a');
//   printf("========================================\n");
//   printf("TESTING PRINT LIBRARY\n");
//   print_library(library);
//     printf("========================================\n");
//   printf("TESTING FIND:\n");
//   found_song = search_song(library, "Adele", "Hello");
//   if(found_song == NULL){
//     printf("song not found\n");
//   }
//   else{
//     printf("song found {%s, %s}\n",found_song->artist,found_song->title);
//   }
//   found_song = search_song(library, "Adele", "Skyfall");
//   if(found_song == NULL){
//     printf("song not found\n");
//   }
//   else{
//     printf("song found {%s, %s}\n",found_song->artist,found_song->title);
//   }
//   printf("========================================\n");
//   printf("TESTING SHUFFLE:\n");
//   shuffle(library,3);
//   printf("========================================\n");
//   printf("TESTING PRINT ARTIST\n");
//   print_artist(library, "Alan Walker");
//   print_artist(library, "Alann Walker");
//   printf("========================================\n");
//   printf("TESTING DELETE SONG\n");
//   delete_song(library, "Alan Walker", "Faded");
//   delete_song(library, "Adele", "Hello");
//   delete_song(library, "Taylor Swift", "Shake it Off");
//   delete_song(library, "Taylor Swift", "Shake it Off");
//   printf("========================================\n");
//   printf("CLEAR LIBRARY:\n");
//   reset(library);
//   print_library(library);
