#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "node.h"
#include "library.h"
#include "commands.h" // song related commands
#include "user.h" // account related commands

int main() {

  struct song_node ** library = init_song_lib();
  struct user ** account_lib = init_acct_lib();
  load_accounts(account_lib);
  int curr_user_index = -1; // current user logged in

  char input[10];

  while (1) {
    printf("=====================\n");
    printf("Your current library:\n");
    print_library(library);

    printf("-------\n");
    printf("What would you like to do? (enter corresponding number/letter)\n");
    printf("-------\n");
    printf("1 - Play song\n");
    printf("2 - Add song\n");
    printf("3 - Remove song\n");
    printf("4 - Create randomized playlist\n");
    printf("5 - Download library\n");
    printf("6 - Clear library\n");
    printf("––\n");
    printf("7 - Login\n");
    printf("8 - Create account\n");
    printf("9 - Save accounts\n");
    if (curr_user_index >= 0) { 
      printf("0 - Delete account\n");
    }
    printf("–\n");
    printf("00 - Delete all accounts\n");
    printf("––\n");
    printf("q - Quit\n");
    printf("-------\n> ");

    scanf("%s", input);
    if (strcmp(input, "00") == 0) {
      delete_accounts();
    }
    else {
      switch (input[0]) {
        case '1':
          play_song();
          break;
        case '2':
          add_song(library);
          update_account(account_lib, curr_user_index, library);
          break;
        case '3':
          remove_song(library);
          update_account(account_lib, curr_user_index, library);
          break;
        case '4':
          randomize_songs(library);
          break;
        case '5':
          download_library(library);
          break;
        case '6':
          printf("Clearing library...\n");
          reset(library);
          printf("Library cleared!\n\n");
          update_account(account_lib, curr_user_index, library);
          break;
        case '7':
          curr_user_index = login(account_lib);
          if (curr_user_index >= 0) {
            library = account_lib[curr_user_index]->library;
          }
          break;
        case '8':
          create_account(account_lib);
          break;
        case '9':
          save_accounts(account_lib);
          printf("\nAccounts saved!\n\n");
          break;
        case '0':
          curr_user_index = delete_account(account_lib, curr_user_index, library);
          break;
        case 'q':
          return 0;
        default:
          printf("\nInvalid command.\n");
          if (curr_user_index < 0) {
            printf("Try: 1, 2, 3, 4, 5, 6, 7, 8, 9, or q\n\n");
          }
          else {
            printf("Try: 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, or q\n\n");
          }
          break;
      }
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
