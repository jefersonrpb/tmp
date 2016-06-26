#ifndef GAME_INCLUDED
#define GAME_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <curses.h>
#include <unistd.h>

typedef struct {
    int y;
    int x;
    int direction;
} Player;

enum direction {UP, DOWN, RIGHT, LEFT};

void set_current_user_locale();
void create_window();
void create_map();
void create_player();
void restore_window();
void quit();
void draw_char(int y, int x, char value);
void update_player();
void update();
void draw();
void tick();

#endif //GAME_INCLUDED
