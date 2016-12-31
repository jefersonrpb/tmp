CC = gcc -std=gnu99
OUTFILE = game
FLAG_LIB = -lncurses
FLAG_DEBUG = -Wall -Werror -ggdb
FLAG_INC = -I inc

SOURCES:=$(shell find src -type f | grep -v "main.c")

all:
	mkdir -p bin/
	$(CC) src/*.c -o bin/$(OUTFILE) -lncurses -I inc

debug: 
	mkdir -p bin/
	$(CC) -Wall -Werror -ggdb src/*.c -o bin/$(OUTFILE) -lncurses -I inc

.PHONY: clean
clean:
	rm -rf bin/*

.PHONY: test
test:
	mkdir -p bin/test
	for path in $$(ls test/*.c); do key=$$(basename $$path .c); echo "instal: $$path";\
		$$($(CC) $(SOURCES) $$path -o bin/test/$$key -lncurses -I inc -Wall -Werror -ggdb); \
	done;\
	echo "run tests";\
	for path in $$(ls test/*.c); do key=$$(basename $$path .c);\
		bin/test/$$key;\
	done


format: 
	astyle --options=$(HOME)/.astyle_format.conf *.c *.h
