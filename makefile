compile runme: main.o node.o library.o
	@gcc -o runme main.o node.o library.o
main.o: main.c node.h library.h
	@gcc -c main.c
library.o: library.c node.h
	@gcc -c library.c
node.o: node.c
	@gcc -c node.c
run: runme
	@./runme
clean:
	@rm *.o
	@rm runme
