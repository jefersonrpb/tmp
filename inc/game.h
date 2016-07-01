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

enum {UP, DOWN, RIGHT, LEFT, WALL, EMPTY, GAME_OVER, PLAY, MENU};

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
Moto *new_player(int x, int y, int direction);
int **create_positions(int length);
void restore_window();
void quit();
void draw_char(int y, int x, char value);
void fulfill_wall(Moto *player);
void update_player();
void draw_rectangle(int x, int y, int width, int height);
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
