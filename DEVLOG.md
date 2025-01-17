# Dev Log:

This document must be updated daily by EACH group member.

### 2025-01-06 - Basic user interface and structure created. Started work on add songs functionality.
Added library display on start and prompts for user input & created function headers for each option user has (play_song, add_song, randomize_songs) (40 min), Created basic functionality where users can manually add a song. (20 min)

### 2025-01-07 - Worked on randomize and play songs functionalities
Added randomize playlist functionality (yet to be tested), started work on play_song() function which allows users to play mp3 files using exec & fork, started looking into id3 tags. (80 min) 

### 2025-01-08 - Worked on saving library, saving randomized playlist, auto add mp3 songs, song playing, remove songs, bug testing
Added save library functionality, added remove song functionality, added basic outline for saving randomized playlist, started work on automatically adding songs functionality (created function for checking if mp3 file or not and started function scanning given directory for mp3 files), tested functions and fixed bugs (input bugs, infinite loop in shuffle()). (2 hr)

### 2025-01-09 - id3v1 and id3v2 extraction, randomize playlist
Added id3v1 metadata extraction function and finished scan directory function and can now auto add songs that way. Tried id3v2 extraction but could not get it to work. Continued to work on randomizing playlist functionality (2.8 hr)

### 2025-01-10 - randomize playlist, remove song update
Continued to work on randomizing playlist functionality -- reworked in_ary function. Updated remove_song() to have separate print statement for when library has no songs. (50 min)

### 2025-01-11 - randomize playlist
Reworked shuffle() to instead remove used songs from within array rather than add to another array and search for if song has been used or not. Also, fixed its not writing to file properly issue. (40 min)

### 2025-01-12 - code clean-up
Added comments, changed formatting of some code, changed some print statements. (20 min)

### 2025-01-13 - bug fixes, started account functions
Fixed issue of the print statements acting as if a song was playing when there wasn't by adding a new function, scan_directory_for_file which checks if mp3 file exists beforehand. Also, fixed weird printing artifact when user tries to add a song w/o inputting anything. Created function headers for the account functionalities. (55 min)

### 2025-01-14 - account functions, clear library, variable renaming
Added init, save_account and load_account and create_account functions, added calls to account functions in main, added clear library functionality in main, renamed LIB_SIZE in commands.c to SONG_COUNT. (1.7 hr)

### 2025-01-15 - account functions cont.
Added login and update_account functions. Tested account functions. Restarted on save_account and load_account functions b/c they were not working. Added load_song_library and save_song_library functions in library.c to be called in the rewrite. save_account and load_account functions still don't work. (3.5 hr)

### 2025-01-16 - account functions cont.
Fixed save account and load account functionalities -- added inc_song_count_by_1() and int get_song_count() in commands.c. Added delete_account and delete_accounts functions. Added functionality to create_account where user can enter q to quit. Created MAX_ARTIST_LEN and MAX_TITLE_LEN macros (2.3 hr)

### 2025-01-17 - Brief description
Expanded description including how much time was spent on task.
