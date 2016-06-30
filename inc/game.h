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
    Point position;
    Point velocity;
    int direction;
} Moto;

enum {UP, DOWN, RIGHT, LEFT, WALL, EMPTY};

#define FPS 1000000 / 10

void update();
void draw();
void tick();
void game_over();
void process_args();
void set_locale();
void create_window();
void create_map();
void create_players();
Moto *new_player(int direction, int x, int y);
int **create_positions(int length);
void restore_window();
void quit();
void draw_char(int y, int x, char value);
void fulfill_wall(Moto *player);
void update_player();
void draw_rectangle(int x, int y, int width, int height);
void input_player_direction(Moto *player);
void update_player_velocity(Moto *player);
void update_player_position(Moto *player);
void check_player_collision(Moto *player);
void update_dumb_ai();
int *get_allowed_directions(Moto *player);

#endif //GAME_INCLUDED
