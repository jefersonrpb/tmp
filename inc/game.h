#ifndef GAME_INCLUDED
#define GAME_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <curses.h>
#include <unistd.h>
#include <time.h>

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    Point *position;
    int direction;
    int color;
    bool dead;
} Moto;

enum directions { UP, DOWN, RIGHT, LEFT };
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
void update_dumb_ai();
void update_player();
void start();
void create_menu();
void draw_char(int y, int x, int value);
void create_bound(int x, int y, int width, int height);
void game_over(bool player_loses);
void fulfill(Moto *player);
void input_player_direction(Moto *player);
void update_player_position(Moto *player);
void check_collisions(Moto *player, bool is_player);
Moto *new_player(int x, int y, enum directions direction, int color);
Point *new_point(int x, int y);
Point *get_next_position(Point *current_position, int direction);
bool check_collision(Point *position);
int *get_allowed_directions(Moto *player);

#endif //GAME_INCLUDED
