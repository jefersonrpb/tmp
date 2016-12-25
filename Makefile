CC = gcc -std=gnu99
OUTFILE = game
FLAG_LIB = -lncurses
FLAG_DEBUG = -Wall -Werror -ggdb
FLAG_INC = -I inc

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

obj/test.o: src/test.c 
	mkdir -p obj/
	$(CC) $^ -c -o $@ $(FLAG_INC)

obj/board.o: src/board.c 
	mkdir -p obj/
	$(CC) src/board.c -c -o $@ $(FLAG_INC) $(FLAG_DEBUG) $(FLAG_LIB)

obj/window.o: obj/board.o src/window.c
	mkdir -p obj/
	$(CC) $^ -c -o $@ $(FLAG_INC) $(FLAG_DEBUG) $(FLAG_LIB)

test_window: obj/test.o obj/window.o obj/board.o
	$(CC) $^ test/window.c -o bin/test/window $(FLAG_INC) $(FLAG_DEBUG) $(FLAG_LIB) 
	bin/test/window

# test_ptr_array: obj/test.o src/ptr_array.c 
# 	$(CC) test/ptr_array.c $^ -o bin/test/ptr_array -lncurses -Wall -Werror -ggdb -I inc
#
# format: # 	astyle --options=$(HOME)/.astyle_format.conf *.c *.h
