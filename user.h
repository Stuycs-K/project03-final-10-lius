#ifndef USER_H
#define USER_H

struct user ** init_acct_lib();
void create_account(struct user ** account_lib);
int login(struct user ** account_lib);
void update_account(struct user ** account_lib, int curr_user_index, struct song_node ** library);
void save_accounts(struct user ** account_lib);
int load_accounts(struct user ** account_lib);
void delete_account(struct user ** account_lib, int curr_user_index);

#endif
