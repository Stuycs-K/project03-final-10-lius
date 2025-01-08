#ifndef COMMANDS_H
#define COMMANDS_H

void play_song();
void add_song(struct song_node ** library);
void randomize_songs(struct song_node ** library);
void save_library(struct song_node ** library);

void scan_directory(char * path);
int is_mp3(char * filename);

#endif
