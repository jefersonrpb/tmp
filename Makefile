CC = gcc -std=gnu99
OUTFILE = game

all:
	mkdir -p bin/
	$(CC) src/*.c -o bin/$(OUTFILE) -lncurses -I inc

debug: 
	mkdir -p bin/
	$(CC) -Wall -Werror -ggdb src/*.c -o bin/$(OUTFILE) -lncurses -I inc

.PHONY: clean
clean:
	rm -rf $(OUTFILE)

.PHONY: test
test_build_all_and_run:
	mkdir -p bin/test
	for path in $$(ls test/*.c); do key=$$(basename $$path .c); echo "instal: $$path";\
		$$($(CC) src/*.c $$path -o bin/test/$$key -lncurses -I inc -Wall -Werror -ggdb); \
	done;\
	echo "run tests";\
	for path in $$(ls test/*.c); do key=$$(basename $$path .c);\
		bin/test/$$key;\
	done

test_ptr_array: src/ptr_array.c 
	$(CC) test/ptr_array.c $^ -o bin/test/ptr_array -lncurses -Wall -Werror -ggdb -I inc

test_int_array: src/int_array.c 
	$(CC) test/int_array.c -o bin/test/int_array $^ -lncurses -Wall -Werror -ggdb -I inc

format:
	astyle --options=$(HOME)/.astyle_format.conf *.c *.h
