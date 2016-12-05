CC = gcc -std=gnu99
OUTFILE = game

all:
	mkdir -p bin/
	$(CC) src/*.c -o bin/$(OUTFILE) -lncurses -I inc

debug: 
	mkdir -p bin/
	$(CC) -Wall -Werror -ggdb src/*.c -o bin/$(OUTFILE) -lncurses -I inc

clean:
	rm -rf $(OUTFILE)
