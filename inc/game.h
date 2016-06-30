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
#define MAP_HEIGHT 20
#define MAP_WIDTH 40 
#define GAME_HEIGHT MAP_HEIGHT + 2
#define GAME_WIDTH MAP_WIDTH + 2

void set_locale();
void create_window();
void create_map();
void create_players();
void initialize_player(Moto *player, int direction, int x, int y);
void restore_window();
void quit();
void draw_char(int y, int x, char value);
void draw_tail(Moto *player);
void update_player();
void update();
void draw();
void tick();
void game_over();
void draw_rectangle(int x, int y, int width, int height);
void input_player_direction(Moto *player);
void update_player_velocity(Moto *player);
void update_player_position(Moto *player);
void check_player_collision(Moto *player);
void update_dumb_ai();
int *get_allowed_directions(Moto *player);

#endif //GAME_INCLUDED
