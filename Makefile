CC = gcc
OUTFILE = game

all:
	mkdir -p bin/
	$(CC) -Wall src/*.c -o bin/$(OUTFILE) -lncurses -I inc

clean:
	rm -rf $(OUTFILE)
