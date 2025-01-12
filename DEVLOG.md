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

### 2025-01-13 - Brief description
Expanded description including how much time was spent on task.

### 2025-01-14 - Brief description
Expanded description including how much time was spent on task.
