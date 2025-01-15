#ifndef USER_H
#define USER_H

struct user ** init_acct_lib();
void create_account(struct user ** account_lib);
void login(struct user ** account_lib);
void update_account(struct user ** account_lib, int curr_user_index, struct song_node ** library); // save to account
void save_accounts(struct user ** account_lib); // back up account to file
void load_accounts(struct user ** account_lib);

#endif
