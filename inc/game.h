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
} Moto;

enum direction {UP, DOWN, RIGHT, LEFT, WALL, EMPTY};

void set_current_user_locale();
void create_window();
void create_map();
void create_players();
void restore_window();
void quit();
void draw_char(int y, int x, char value);
void update_player();
void update();
void draw();
void tick();
void game_over();
void find_player_direction();
void update_player_position();
void check_player_collision();

#endif //GAME_INCLUDED
