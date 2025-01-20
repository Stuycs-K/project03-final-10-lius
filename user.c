#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "library.h"
#include "user.h"

#define SAVED_ACCOUNTS "accounts.dat"
#define MAX_USERS 100

static int total_users = 0; // current amount of created accounts

/* Intializes array of users.
 * Sets all indexes to NULL.
*/
struct user ** init_acct_lib() {
  struct user ** user_arr = (struct user **)malloc(MAX_USERS*sizeof(struct user));
  for (int i = 0; i < MAX_USERS; i ++) {
    user_arr[i] = NULL;
  }
  return user_arr;
}

/* Creates account using user inputted username and password.
 * Once created, adds account to account_lib array and backups it up to file.
*/
void create_account(struct user ** account_lib) {
  printf("enter 'q' to quit\n");
  // get username
  char username[MAX_USERNAME_LEN + 1];  // +1 for null terminator
  getchar(); // clear newline character left by previous scanf
  while (1) {
    printf("Enter username (%d char max): ", MAX_USERNAME_LEN);
    fflush(stdin); // clear input buffer
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0'; // remove newline character if present

    if (strcmp(username, "q") == 0) {
      return;
    }

    if (strlen(username) >= MAX_USERNAME_LEN) {
      printf("Username too long.\n");
      goto restart_username; // go back to start of while loop
    }
    if (strlen(username) <= 1) {
      printf("Username too short.\n");
      goto restart_username;
    }

    for (int i = 0; i < total_users; i++) {
      if (strcmp(username, account_lib[i]->username) == 0) {
        printf("Username already exists. Try a different one.\n");
        goto restart_username;
      }
    }

    break;

    restart_username:;
  }

  // get password
  char password[MAX_PWD_LEN];
  while (1) {
    printf("Enter password (%d char max): ", MAX_PWD_LEN);
    fflush(stdin); // clear input buffer
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0'; // remove newline character if present

    if (strcmp(password, "q") == 0) {
      return;
    }

    if (strlen(password) >= MAX_USERNAME_LEN) {
      printf("Password too long.\n");
      goto restart_pwd; // go back to start of while loop
    }
    if (strlen(password) <= 3) {
      printf("Password too short.\n");
      goto restart_pwd;
    }

    break;

    restart_pwd:;
  }

  // create new account
  struct user * new_user = (struct user *)malloc(sizeof(struct user));
  strcpy(new_user->username, username);
  strcpy(new_user->password, password);
  new_user->library = init_song_lib();

  // store account in account library
  account_lib[total_users] = new_user;
  total_users++;

  // backup account to file
  save_accounts(account_lib);

  printf("Account created!\n\n");
}

/* "Logs" user in if account w/ inputted username and password exists.
 * Returns the index of the user who logged in.
 * Returns -1 if unsuccessful log in.
*/
int login(struct user ** account_lib) {
  getchar(); // clear newline character left by previous scanf
  char username[MAX_USERNAME_LEN];
  printf("Enter username: ");
  scanf("%[^\n]", username);

  getchar();
  char password[MAX_PWD_LEN];
  printf("Enter password: ");
  scanf("%[^\n]", password);

  for (int i = 0; i < total_users; i++) {
    if ((strcmp(username, account_lib[i]->username) == 0) && strcmp(password, account_lib[i]->password) == 0) {
      printf("\nLogin successful!\n\n");
      return i;
    }
  }

  printf("\nInvalid username or password.\n\n");
  return -1;
}

/* Saves song library locally to logged in account.
 * int curr_user_index: current user logged in
*/
void update_account(struct user ** account_lib, int curr_user_index, struct song_node ** library) {
  if (curr_user_index == -1) {
    return; // not logged in
  }
  else {
    account_lib[curr_user_index]->library = library;
  }
}

/* Backups account(s) to file */
void save_accounts(struct user ** account_lib) {
  FILE * file = fopen(SAVED_ACCOUNTS, "wb");
  if (file == NULL) {
    printf("Error saving accounts to file\n");
    return;
  }

  // iterate over each user and save their data and song library
  for (int i = 0; i < total_users; i++) {

    // save username and password
    fwrite(account_lib[i], sizeof(struct user), 1, file);

    // save song library
    save_song_library(account_lib[i]->library, file);

    // separate structs
    fputc('a', file);

    //printf("Saved account %s\n", account_lib[i]->username);
  }

  fclose(file);
}

/* Reads account(s) from file.
 * Returns total number of users saved in account file.
 * If no file, prints error and returns 0.
*/
int load_accounts(struct user ** account_lib) {
  FILE * file = fopen(SAVED_ACCOUNTS, "rb");
  if (file == NULL) {
    perror("Error opening accounts data file");
    return 0;
  }

  struct user * temp_user;
  while (1) {
    temp_user = (struct user *)malloc(sizeof(struct user));

    // load username and password
    size_t read_size = fread(temp_user, sizeof(struct user), 1, file);
    if (read_size == 0) {
      free(temp_user);  // free memory if no data is read
      break;
    }

    // load song library
    temp_user->library = init_song_lib();
    load_song_library(temp_user->library, file);

    // store account in account library
    account_lib[total_users] = temp_user;
    total_users++;

    printf("Loaded account %s\n", temp_user->username);
  }

  fclose(file);
  return total_users;
}

/* Deletes logged in account.
 * Removes account from account_lib and backup file.
 * Removes all songs in library.
*/
int delete_account(struct user ** account_lib, int curr_user_index, struct song_node ** library) {
  if (total_users < 0) {
    printf("\nNo accounts to delete.\n\n");
  }
  else {
    if (curr_user_index < 0) {
      printf("\nYou must be logged in to delete an account.\n\n");
    }
    else {
      // remove account and shift elements to left to fill gap
      for (int i = curr_user_index; i < total_users-1; i++) {
        account_lib[i] = account_lib[i + 1];
      }
      total_users--;

      save_accounts(account_lib);

      reset(library);
    }
  }
  return -1;
}

/* Deletes account backup file and exits user */
void delete_accounts() {
  printf("\nAre you sure you want to delete all accounts? (yes/no)\n");
  printf("> ");

  getchar(); // clear newline character left by previous scanf
  char input[10];
  scanf("%[^\n]", input);
  if (strcmp(input, "yes") == 0) {
    printf("\nDeleting accounts...\n");
    remove(SAVED_ACCOUNTS);
    printf("Deleted accounts!\n");
    printf("\nExiting...\n\n");
    exit(0);
  }
}

/* Displays all accounts (and their data) stored in account_lib */
void display_accounts(struct user ** account_lib) {
  printf("\n==============\n");
  printf("-- Accounts --\n");
  for (int i = 0; i < total_users; i++) {
    printf("Username: %s, Password: %s\n", account_lib[i]->username, account_lib[i]->password);
    printf("Library:\n");
    print_library(account_lib[i]->library);
  }
  printf("==============\n");
}

/* Initial interface for display and delete accounts */
void admin(struct user ** account_lib) {
  getchar(); // clear newline character left by previous scanf
  char password[MAX_PWD_LEN];
  printf("Enter admin password: ");
  scanf("%[^\n]", password);
  if (strcmp(password, "password") != 0) {
    printf("\nIncorrect password.\n\n");
  }
  else {
    while (1) {
      printf("=====================\n");
      printf("\n-- Welcome --\n");
      char input[1];
      printf("-------\n");
      printf("1 - Display accounts\n");
      printf("2 - Delete accounts\n");
      printf("q - Quit\n");
      printf("-------\n> ");

      scanf("%s", input);

      if (strlen(input) == 1) {
        switch (input[0]) {
          case 1:
            display_accounts(account_lib);
            break;
          case 2:
            delete_accounts();
            break;
          case 'q':
            return;
          default:
            printf("\nInvalid command.\n");
            break;
        }
      }
      else {
        printf("\nInvalid command.\n");
      }
    }
  }
}