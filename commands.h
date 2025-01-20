#ifndef COMMANDS_H
#define COMMANDS_H

#define RAND_LST_SAVE "downloads/randomized_playlist_save.txt"

void inc_song_count_by_1();
int get_song_count();

void play_song();
void add_song(struct song_node ** library);
void remove_song(struct song_node ** library);
void randomize_songs(struct song_node ** library);
void download_library(struct song_node ** library);
void search(struct song_node ** library);
void invalid_cmd(int curr_user_index);

int is_mp3(char * filename);
void scan_directory_to_extract(char * path, struct song_node ** library);
int scan_directory_for_file(char * dir_path, char * file);
void scan_directory_to_print(char * path);
void extract_metadata_id3v1(char * file_path, struct song_node ** library);

char * concat(char * s1, char * s2);
void remove_whitespace(char * str);

#endif