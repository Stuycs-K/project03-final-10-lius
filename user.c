#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "library.h"
#include "user.h"

#define SAVED_ACCOUNTS "accounts.dat"
#define MAX_USERS 100

static int total_users = 0; // current amount of created accounts

/* Intialize array of users.
 * Sets all indexes to NULL.
*/
struct user ** init_acct_lib() {
  struct user ** user_arr = (struct user **)malloc(MAX_USERS*sizeof(struct user));
  for (int i = 0; i < MAX_USERS; i ++) {
    user_arr[i] = NULL;
  }
  return user_arr;
}

void create_account(struct user ** account_lib) {
  // get username
  char username[MAX_USERNAME_LEN + 1];  // +1 for null terminator
  getchar();
  while (1) {
    printf("Enter username (%d char max): ", MAX_USERNAME_LEN);
    //getchar();
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

  save_accounts(account_lib);

  printf("Account created!\n");
}

/* Returns the index of the user who logged in.
 * Returns -1 if unsuccessful log in.
*/
int login(struct user ** account_lib) {
  getchar(); // clear newline character left by previous scanf
  char username[256];
  printf("Enter username: ");
  scanf("%[^\n]", username);

  getchar();
  char password[256];
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

/* Save song library locally to account.
 * int curr_user_index: current user logged in
*/
void update_account(struct user ** account_lib, int curr_user_index, struct song_node ** library) {
  if (curr_user_index == -1) {
    return;
  }
  else {
    //printf("autosaving...\n");
    account_lib[curr_user_index]->library = library;
  }
}

/* Backup account(s) to file */
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
    fputc('\n', file);

    printf("Saved account %s\n", account_lib[i]->username);
  }

  fclose(file);
}


/* Read account(s) from file
 * Returns total number of users saved in account file
 * If no file, prints error and returns 0
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

int delete_account(struct user ** account_lib, int curr_user_index, struct song_node ** library) {
  for (int i = curr_user_index; i < curr_user_index-1; i++) {
    account_lib[i] = account_lib[i + 1];
  }
  save_accounts(account_lib);
  reset(library);
  
  return -1;
}

void delete_accounts() {
  getchar();
  char password[256];
  printf("Enter admin password: ");
  scanf("%[^\n]", password);
  if (strcmp(password, "password") == 0) {
    printf("\n-Welcome-\n");
    printf("\nAre you sure you want to delete all accounts? (yes/no)\n");
    printf("> ");
    getchar();
    char input[10];
    scanf("%[^\n]", input);
    if (strcmp(input, "yes") == 0) {
      printf("\nDeleting accounts\n");
      remove(SAVED_ACCOUNTS);
      printf("Deleted Accounts\n\n");
      exit(0);
    }
  }
}
