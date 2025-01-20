#ifndef USER_H
#define USER_H

#define MAX_USERNAME_LEN 100
#define MAX_PWD_LEN 256

struct user {char username[MAX_USERNAME_LEN]; char password[MAX_PWD_LEN]; struct song_node ** library;};

struct user ** init_acct_lib();

void create_account(struct user ** account_lib);
int login(struct user ** account_lib, int curr_user_index);

void update_account(struct user ** account_lib, int curr_user_index, struct song_node ** library);
void save_accounts(struct user ** account_lib);
int load_accounts(struct user ** account_lib);

int delete_account(struct user ** account_lib, int curr_user_index, struct song_node ** library);

void delete_accounts();
void display_accounts(struct user ** account_lib);
void admin(struct user ** account_lib);

#endif