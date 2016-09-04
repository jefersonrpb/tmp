#ifndef GAME_INCLUDED
#define GAME_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <curses.h>
#include <unistd.h>
#include <time.h>

typedef struct {
    int position;
    int direction;
    int color;
    bool alive;
} LightCycle;

enum directions {LEFT = 0, UP = 1, RIGHT = 2, DOWN = 3};
enum states { PLAY, GAME_OVER, MENU };
enum draw { 
    WALL = '#', 
    EMPTY = ' ',
    BOUND_CORNER = '+',    
    BOUND_LINE_VERTICAL = '|',
    BOUND_LINE_HORIZONTAL = '-'
};

#define SECOND 1000000
#define FPS SECOND / 10

void update();
void draw();
void tick();
void quit();
void create_window();
void create_map();
void create_players();
void restore_window();
void update_player();
void start();
void create_menu();
void draw_char(int y, int x, int value);
void create_bound(int x, int y, int width, int height);
void game_over(bool player_loses);
void fulfill(LightCycle *player);
void input_player_direction(LightCycle *player);
LightCycle *new_player(int x, int y, enum directions direction, int color);
int *get_allowed_directions(LightCycle *player);

#endif //GAME_INCLUDED
