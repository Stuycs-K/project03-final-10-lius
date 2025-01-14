#include <stdio.h>
#include <stdlib.h>

#define SAVED_ACCOUNTS "accounts.dat"
#define MAX_USERS 100

//struct user library[MAX_USERS];
int curr_user_count = 0;

struct user {char username[100]; char password[100]; struct song_node ** library;};

struct user ** init() {
  struct user ** user_arr = (struct user **)malloc(MAX_USERS*sizeof(struct user));
  for (int i = 0; i < MAX_USERS; i ++) {
    user_arr[i] = NULL;
  }
  return user_arr;
}

void create_account();
void login();
void update_account(); //save to account

void save_account(struct user ** account_lib) {
  FILE * file = fopen(SAVED_ACCOUNTS, "wb");
  if (file == NULL) {
    printf("Error saving accounts to file\n");
    return;
  }

  fwrite(account_lib, sizeof(struct user *), curr_user_count, file);
  fclose(file);
}

void load_accounts(struct user ** account_lib) {
  FILE * file = fopen(SAVED_ACCOUNTS, "rb");
  if (file == NULL) {
    perror("Error opening accounts data file"); // no file or unable to open, no accounts to load
    return;
  }

  while (fread(&account_lib[curr_user_count], sizeof(struct user *), 1, file) == 1) {
    curr_user_count++;
  }

  fclose(file);
}
