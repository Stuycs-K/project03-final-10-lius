compile runme: main.o node.o library.o commands.o user.o
	@gcc -o runme main.o node.o library.o commands.o user.o
main.o: main.c node.h library.h commands.h user.h
	@gcc -c main.c
commands.o: commands.c node.h library.h
	@gcc -c commands.c
user.o: user.c 
	@gcc -c user.c
library.o: library.c node.h commands.h
	@gcc -c library.c
node.o: node.c
	@gcc -c node.c
run: runme
	@./runme
clean:
	@rm *.o
	@rm runme
	@rm library_save.txt randomized_playlist_save.txt accounts.dat
