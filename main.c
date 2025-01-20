#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include "node.h"
#include "library.h"
#include "commands.h" // song related commands
#include "user.h" // account related commands

struct user ** account_lib;

static void sighandler(int signo) {
  if (signo == SIGINT) {
    save_accounts(account_lib);
    printf("\n_____________________");
    exit(0);
  }
}

int main() {
  struct song_node ** library = init_song_lib();
  account_lib = init_acct_lib();
  load_accounts(account_lib);
  int curr_user_index = -1; // current user logged in

  char input[10];

  while (1) {
    signal(SIGINT, sighandler);
    
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
    printf("f - Search library\n");
    printf("––\n");
    printf("7 - Login\n");
    printf("8 - Create account\n");
    printf("9 - Save accounts\n");
    if (curr_user_index >= 0) { 
      printf("0 - Delete account\n");
    }
    printf("––\n");
    printf("p - Admin\n");
    printf("––\n");
    printf("q - Quit\n");
    printf("-------\n> ");

    scanf("%s", input);
    
    if (strlen(input) == 1) {
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
          printf("\nClearing library...\n");
          reset(library);
          printf("Library cleared!\n\n");
          update_account(account_lib, curr_user_index, library);
          break;
        case 'f':
          search(library);
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
          printf("\nSaving accounts...\n");
          save_accounts(account_lib);
          printf("\nAccounts saved!\n\n");
          break;
        case '0':
          curr_user_index = delete_account(account_lib, curr_user_index, library);
          break;
        case 'p':
          admin(account_lib);
          break;
        case 'q':
          save_accounts(account_lib);
          printf("_____________________\n");
          return 0;
        default:
          invalid_cmd(curr_user_index);
          break;
      }
    }
    else {
      invalid_cmd(curr_user_index);
    }
  }

  return 0;

}