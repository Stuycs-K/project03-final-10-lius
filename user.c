#include <stdio.h>
#include <stdlib.h>

#define SAVED_ACCOUNTS "accounts.dat"
#define MAX_USERS 100
#define MAX_USERNAME_LEN 100
#define MAX_PWD_LEN 256

static int total_users = 0; // current amount of created accounts

struct user {char username[MAX_USERNAME_LEN]; char password[MAX_PWD_LEN]; struct song_node ** library;};

struct user ** init() {
  struct user ** user_arr = (struct user **)malloc(MAX_USERS*sizeof(struct user));
  for (int i = 0; i < MAX_USERS; i ++) {
    user_arr[i] = NULL;
  }
  return user_arr;
}

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

  save_accounts(account_lib);
  // FILE * file = fopen(SAVED_ACCOUNTS, "wb");
  // fwrite(account_lib[0], sizeof(struct user), 1, file);
  // fclose(file);

  printf("Account created!\n");
}

void login();
void update_account(); //save to account

void save_account(struct user ** account_lib) {
  FILE * file = fopen(SAVED_ACCOUNTS, "wb");
  if (file == NULL) {
    printf("Error saving accounts to file\n");
    return;
  }

  fwrite(account_lib, sizeof(struct user *), total_users, file);
  fclose(file);
}

void load_accounts(struct user ** account_lib) {
  FILE * file = fopen(SAVED_ACCOUNTS, "rb");
  if (file == NULL) {
    perror("Error opening accounts data file"); // no file or unable to open, no accounts to load
    return;
  }

  while (fread(&account_lib[total_users], sizeof(struct user *), 1, file) == 1) {
    total_users++;
  }

  fclose(file);
}
