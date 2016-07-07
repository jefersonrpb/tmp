#ifndef GAME_INCLUDED
#define GAME_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <locale.h>
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
void game_over(bool player_loses);
void process_args();
void set_locale();
void create_window();
void create_map();
void create_players();
Moto *new_player(int x, int y, enum directions direction);
int **create_positions(int length);
void restore_window();
void quit();
void draw_char(int y, int x, char value);
void fulfill_wall(Moto *player);
void update_player();
void create_bound(int x, int y, int width, int height);
void input_player_direction(Moto *player);
void update_player_position(Moto *player);
void update_dumb_ai();
int *get_allowed_directions(Moto *player);
bool check_collision(Point *position);
Point *get_next_position(Point *current_position, int direction);
Point *new_point(int x, int y);
void start();
void create_menu();

#endif //GAME_INCLUDED
