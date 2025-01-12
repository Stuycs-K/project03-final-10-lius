#ifndef COMMANDS_H
#define COMMANDS_H

void play_song();
void add_song(struct song_node ** library);
void remove_song(struct song_node ** library);
void randomize_songs(struct song_node ** library);
void save_library(struct song_node ** library);

char * concat(char * s1, char * s2);

int is_mp3(char * filename);
void scan_directory(char * path, struct song_node ** library);
void extract_metadata_id3v1(char * file_path, struct song_node ** library);
void extract_metadata_id3v2(char * file_path, struct song_node ** library);

#endif
