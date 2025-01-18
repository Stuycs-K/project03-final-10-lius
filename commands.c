#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <sys/wait.h>
#include "node.h"
#include "library.h"
#include "commands.h"

#define MP3_FILES_DIR_PATH "songs/"
#define LIB_SAVE "library_save.txt"

static int song_count = 0;

int get_song_count() {
  return song_count;
}

void inc_song_count_by_1() {
  song_count++;
}

/* Plays user inputted song by using mpg123 */
void play_song() {
  getchar(); // clear newline character left by previous scanf
  char * song = (char *)malloc(256);
  printf("Enter song to play: ");
  scanf("%[^\n]", song);
  if (!is_mp3(song)) {
    song = concat(song, ".mp3");
  }

  if (!scan_directory_for_file(MP3_FILES_DIR_PATH, song)) {
    printf("\nAn MP3 file for that song does not exist.\n");
    printf("\nAvailable songs:\n");
    scan_directory_to_print(MP3_FILES_DIR_PATH);
    printf("\n");
    return;
  }

  char * song_path = concat(MP3_FILES_DIR_PATH, song);

  pid_t pid = fork();
  if (pid < 0) {
    perror("fork fail\n");
    return;
  }
  if (pid == 0) {
    // prepare argument list for exec
    char *args[] = {"mpg123", "-q", song_path, NULL};

    // execute mpg123 program to play the MP3 file
    execvp("mpg123", args);
  }
  else { // parent
    printf("-\n");
    printf("Playing %s...\n", song);
    printf("Press 'q' to quit and 'space' to pause. 'h' for more functions\n");
    printf("-\n");
    wait(NULL);
  }

  // free memory allocated by concat
  free(song);
  free(song_path);

  printf("\n");
}

/* Adds song(s) to library */
void add_song(struct song_node ** library) {
  char input[1];
  printf("\nWould you like to manually add a song or automatically input one via MP3 file?\n");
  printf("-------\n");
  printf("1 - Manual\n");
  printf("2 - Automatic\n");
  printf("-------\n> ");

  scanf("%s", input);

  switch (input[0]) {
    case '1':
      // manual
      getchar(); // clear newline character left by previous scanf
      char * title = (char *)malloc(MAX_TITLE_LEN);
      printf("Enter song title: ");
      int valid_input = scanf("%[^\n]", title);
      if (!valid_input) title = "";

      getchar();
      char * artist = (char *)malloc(MAX_ARTIST_LEN);;
      printf("Enter artist name: ");
      valid_input = scanf("%[^\n]", artist);
      if (!valid_input) artist = "";

      add(library, artist, title);

      song_count++;
      break;
    case '2':
      // auto
      printf("\nAdding songs...\n");
      scan_directory_to_extract(MP3_FILES_DIR_PATH, library);
      break;
    default:
      printf("\nInvalid command.\n");
      break;
  }

  printf("\n");
}

/* Removes user inputted song from library */
void remove_song(struct song_node ** library) {
  if (song_count <= 0) {
    printf("\nThere are no songs to remove.\n\n");
    return;
  }
  else {
    getchar(); // clear newline character left by previous scanf
    char * title = (char *)malloc(MAX_TITLE_LEN);
    printf("Enter song title: ");
    int valid_input = scanf("%[^\n]", title);
    if (!valid_input) title = "";

    getchar();
    char * artist = (char *)malloc(MAX_ARTIST_LEN);;
    printf("Enter artist name: ");
    valid_input = scanf("%[^\n]", artist);
    if (!valid_input) artist = "";

    int has_song = delete_song(library, artist, title);
    if (has_song) {
      printf("\nRemoving: {%s, %s} \n\n", artist, title);

      song_count--;
    }
    else {
      printf("\nSong not found.\n\n");
      return;
    }
  }
}

/* Prints songs from library in a randomized order */
void randomize_songs(struct song_node ** library) {
  printf("\n");
  if (song_count <= 0) {
    printf("There are no songs to create a randomized playlist with.\n\n");
    return;
  }
  else {
    printf("Creating randomized playlist...\n");
    printf("\n--- Your Randomized Playlist ---\n");
    shuffle(library);

    char input[10];
    printf("Would you like to download this playlist? (yes/no)\n");
    printf("> ");
    getchar(); // clear newline character left by previous scanf
    scanf("%[^\n]", input);
    if (strcmp(input, "yes") == 0) {
      printf("\nDownloaded randomized playlist!\n");
      return;
    }
    else {
      if (remove(RAND_LST_SAVE) != 0) {
        perror("Error deleting file for randomized playlist\n");
      }
      printf("\n");
    }
  }
}

/* Saves library to text file */
void download_library(struct song_node ** library) {
  printf("\n");
  printf("Downloading library...\n");

  int fd = open(LIB_SAVE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fd == -1) {
    perror("Error opening file to save library\n");
    return;
  }

  for (int i = 0; i < LIB_SIZE; i++) {
    if (library[i] != NULL) {
      dprintf(fd, "%c: ", i + 96);
      song_list_to_file(library[i], fd);
    }
  }
  close(fd);

  printf("\nLibrary downloaded!\n\n");
}

/* Searches for song or songs of an artist in library.
 * Case-sensitive.
*/
void search(struct song_node ** library) {
  char input[1];
  printf("\nFind...\n");
  printf("-------\n");
  printf("1 - Artist\n");
  printf("2 - Song\n");
  printf("-------\n> ");

  scanf("%s", input);

  switch (input[0]) {
    case '1':
      // find artist
      getchar(); // clear newline character left by previous scanf
      char * artist = (char *)malloc(MAX_ARTIST_LEN);
      printf("Enter artist: ");
      int valid_input = scanf("%[^\n]", artist);
      if (!valid_input) artist = "";

      print_artist(library, artist);
      break;
    case '2':
      // find song
      getchar();
      char * title = (char *)malloc(MAX_TITLE_LEN);
      printf("Enter song title: ");
      valid_input = scanf("%[^\n]", title);
      if (!valid_input) title = "";

      getchar();
      printf("Enter artist name: ");
      valid_input = scanf("%[^\n]", artist);
      if (!valid_input) artist = "";

      if (search_song(library, artist, title) == NULL) {
        printf("\nSong not found.\n");
      }
      else {
        printf("\nSong found!\n");
        printf("%c: ", artist[0]);
        print_song_list(library[first_letter(artist[0])]);
      }
      break;
    default:
      printf("\nInvalid command.\n");
      break;
  }

  printf("\n");
}

/* Determines if file is an MP3 file or not.
 * Returns true if file is an MP3 file.
 */
int is_mp3(char * filename) {
  char * extension = strrchr(filename, '.');
  return (extension && strcmp(extension, ".mp3") == 0); // check if has extension and that extension is an mp3
}

/* Extracts ID3v1 tag from the end of MP3 file.
 * Gets MP3 file from given file path.
 * Uses extracted data (artist and title) to add song to library.
 * If song already added to library, exit.
*/
void extract_metadata_id3v1(char * file_path, struct song_node ** library) {
  FILE * file = fopen(file_path, "rb");

  fseek(file, -128, SEEK_END);
  char tag[128];
  fread(tag, 1, 128, file);

  if (memcmp(tag, "TAG", 3) == 0) {
    // extract title and artist from the ID3v1 tag
    char title[31], artist[31];
    memcpy(title, tag + 3, 30);
    memcpy(artist, tag + 33, 30);

    title[30] = '\0';
    artist[30] = '\0';

    remove_whitespace(title);
    remove_whitespace(artist);

    // printf("00%s00, ", artist);
    // printf("00%s00\n", title);
    
    if (search_song(library, artist, title) != NULL){
      return;
    }
    add(library, artist, title);
  } else {
    printf("ID3v1 tag not found in file: %s\n", file_path);
  }
  fclose(file);

  song_count++;
}

// void extract_metadata_id3v2(char * file_path, struct song_node ** library) {
//   FILE * file = fopen(file_path, "rb");

//   char header[10];
//   fseek(file, 0, SEEK_SET);
//   fread(header, 1, 10, file);
//   if (memcmp(header, "ID3", 3) == 0) {
//     unsigned int size = ((header[6] & 0x7f) << 21) | ((header[7] & 0x7f) << 14) | ((header[8] & 0x7f) << 7) | (header[9] & 0x7f);

//     // read the rest of the ID3v2 frame data
//     unsigned char *frame_data = malloc(size);
//     fread(frame_data, 1, size, file);

//     // ID3v2 frame format: [frame id (4 bytes)][size (4 bytes)][data]
//     unsigned int offset = 0;

//     char * title;
//     char * artist;

//     // search for "TIT2" (Title frame) and "TPE1" (Artist frame) in the frame data
//     while (offset + 10 <= size) {
//       char frame_id[5];
//       unsigned int frame_size;
//       memcpy(frame_id, frame_data + offset, 4);
//       frame_id[4] = '\0';
//       frame_size = (frame_data[offset + 4] << 24) | (frame_data[offset + 5] << 16) | (frame_data[offset + 6] << 8) | frame_data[offset + 7];

//       printf("%s\n", frame_id);

//       if (strcmp(frame_id, "TIT2") == 0) {
//         title = (char *)malloc(frame_size + 1);
//         memcpy(title, frame_data + offset + 10, frame_size);
//         title[frame_size] = '\0';
//         printf("Title: %s\n", title);
//         //free(title);
//       } else if (strcmp(frame_id, "TPE1") == 0) {
//         artist = (char *)malloc(frame_size + 1);
//         memcpy(artist, frame_data + offset + 10, frame_size);
//         artist[frame_size] = '\0';
//         printf("Artist: %s\n", artist);
//         //free(artist);
//       }

//       offset += 10 + frame_size;
//     }
//     free(frame_data);
//     add(library, artist, title);
//   }
//   else {
//     printf("ID3v2 tag not found in file: %s\n", file_path);
//   }
// fclose(file);
// song_count++;
// }

/* Scans for MP3 files in given directory.
 * If finds MP3 file, gets its title and artist.
*/
void scan_directory_to_extract(char * path, struct song_node ** library) {
  int has_mp3 = 0;

  DIR * d = opendir(path);
  if (path == NULL) {
    perror("Error opening directory\n");
    return;
  }

  struct dirent * entry;
  while ((entry = readdir(d)) != NULL) {
    if (entry->d_type == DT_REG && is_mp3(entry->d_name)) {
      has_mp3 = 1;
      char * file_path = concat(path, entry->d_name);

      //extract_metadata_id3v2(file_path, library);
      extract_metadata_id3v1(file_path, library);

      free(file_path); // free memory allocated by concat
    }
  }

  if (!has_mp3) {
    printf("No MP3 files found.\n");
  }
}

/* Iterates through files in given directory.
 * Returns true if given file is in directory.
*/
int scan_directory_for_file(char * dir_path, char * file) {
  DIR * d = opendir(dir_path);
  if (dir_path == NULL) {
    perror("Error opening directory\n");
    return 0;
  }

  struct dirent * entry;
  while ((entry = readdir(d)) != NULL) {
    if (entry->d_type == DT_REG) {
      if (strcmp(file, entry->d_name) == 0) {
        return 1;
      }
    }
  }
  return 0;
}

/* Scans for MP3 files in given directory.
 * If finds MP3 file prints it
*/
void scan_directory_to_print(char * path) {
  DIR * d = opendir(path);
  if (path == NULL) {
    perror("Error opening directory\n");
    return;
  }

  struct dirent * entry;
  while ((entry = readdir(d)) != NULL) {
    if (entry->d_type == DT_REG && is_mp3(entry->d_name)) {
      printf("- %s\n", entry->d_name);
    }
  }
}

/* Combines given 2 strings. First given string paramater is the start of the new combined string.
 * Returns combined string.
 */
char * concat(char * s1, char * s2) {
  char * cat_str = (char *)malloc(strlen(s1) + strlen(s2) + 1);
  strcpy(cat_str, s1);
  strcat(cat_str, s2);
  return cat_str;
}

/* Remove leading and trailing spaces from given string */
void remove_whitespace(char * str) {
  while (*str == ' ') str++;
  char * end = str + strlen(str) - 1;
  while (end > str && *end == ' ') end--;
  *(end + 1) = '\0';
}