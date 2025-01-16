#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "library.h"

#include <unistd.h>
#include <fcntl.h>

#define SAVED_ACCOUNTS "accounts.dat"
#define MAX_USERS 100
#define MAX_USERNAME_LEN 100
#define MAX_PWD_LEN 256

//struct user library[MAX_USERS];
int total_users = 0; // current amount of created accounts . 

struct user {char username[MAX_USERNAME_LEN]; char password[MAX_PWD_LEN]; struct song_node ** library;};

// intialize array of users. sets all indexes to NULL
struct user ** init_acct_lib() {
  struct user ** user_arr = (struct user **)malloc(MAX_USERS*sizeof(struct user));
  for (int i = 0; i < MAX_USERS; i ++) {
    user_arr[i] = NULL;
  }
  return user_arr;
}

void save_accounts(struct user ** account_lib);

void create_account(struct user ** account_lib) {
  // get username
  char username[MAX_USERNAME_LEN + 1];  // +1 to account for the null terminator

  int n = 1;

  while (n) { //make it so it exits on 'q'
    printf("Enter username (%d char max): ", MAX_USERNAME_LEN);
    getchar(); // clear newline character left by previous scanf
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0'; // remove newline character if present

    if (strlen(username) >= MAX_USERNAME_LEN) {
      printf("Username too long. (%lu characters over)\n", strlen(username)-MAX_USERNAME_LEN);
      continue;
      //go back to start of while loop
    }
    if (strlen(username) <= 1) {
      printf("Username too short.");
      continue;
    }

    //printf("Aẞ");

    for (int i = 0; i < total_users; i++) {
      if (strcmp(username, account_lib[i]->username) == 0) {
          printf("Username already exists. Try a different one.\n");
          continue;
          //go back to start of while loop
      }
    }
    //printf("Aẞ");
    n = 0;
    //printf("Aẞ");
  }

  //printf("Aẞ");

  // get password
  //printf("Aẞ");
  //getchar();
  char password[MAX_PWD_LEN];
  //printf("Aẞ");'
  //printf("Enter password: ");
  //scanf("%[^\n]", password);

  n = 1;

  while (n) {
    printf("Enter password (%d char max): ", MAX_PWD_LEN);
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0'; // remove newline character if present

    if (strlen(password) >= MAX_USERNAME_LEN) {
      printf("Password too long. (%lu characters over)\n", strlen(password)-MAX_USERNAME_LEN);
      continue;
      //go back to start of while loop
    }
    //printf("%s\n", password);
    if (strlen(password) <= 3) {
      printf("Password too short.\n"); //bug when this happens
      continue;
    }
    n = 0;
  }

  struct user * new_user = (struct user *)malloc(sizeof(struct user));
  strcpy(new_user->username, username);
  strcpy(new_user->password, password);
  struct user ** library = NULL;

  account_lib[total_users] = new_user;

  total_users++;

  //save_accounts(account_lib);
  FILE * file = fopen(SAVED_ACCOUNTS, "wb");
  fwrite(account_lib[0], sizeof(struct user), 1, file);
  fclose(file);

  printf("Account created!\n");
}

/* Returns the index of the user who logged in.
 * Returns 0 if unsuccessful log in.
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

  //printf("STUFF: %d, %s, %s,\n", total_users, username, password);
  //printf("%s", account_lib[0]->username);
  for (int i = 0; i < total_users; i++) {
    //printf("STUFF: %s, %s,\n", account_lib[i]->username, account_lib[i]->password);
    if ((strcmp(username, account_lib[i]->username) == 0) && strcmp(password, account_lib[i]->password) == 0) {
      printf("Login successful!\n");
      return i;
    }
  }

  printf("Invalid username or password\n");
  return 0;
}

/* save library to account */
//curr_user_index is current user logged in
void update_account(struct user ** account_lib, int curr_user_index, struct song_node ** library) {
  printf("USR: %d\n", curr_user_index);
  if (curr_user_index == -1) {
    return;
  }
  else {
    printf("autosaving...\n");
    account_lib[curr_user_index]->library = library;
    print_library(account_lib[curr_user_index]->library);
  }
}

/* backup accounts to file */
void save_accounts(struct user ** account_lib) {
  FILE * file = fopen(SAVED_ACCOUNTS, "wb");
  if (file == NULL) {
      printf("Error saving accounts to file\n");
      return;
  }

  for (int i = 0; i < total_users; i++) {
    fwrite(account_lib[i], sizeof(struct user), 1, file);

    save_song_library(account_lib[i]->library, file);

    printf("Saved user %s and their song library\n", account_lib[i]->username);
  }

  fclose(file);
  printf("Accounts and libraries saved!\n");
}


/* Returns total number of users saved in account file 
 * If no file, 0
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
          free(temp_user);
          break;
      }

      temp_user->library = init_song_lib();
      load_song_library(temp_user->library, file);
      print_library(temp_user->library);

      account_lib[total_users] = temp_user;
      total_users++;

      printf("Loaded user %s and their song library\n", temp_user->username);
  }

  fclose(file);
  return total_users;

}

void delete_account(struct user ** account_lib, int curr_user_index);