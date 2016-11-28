#include "game.h"

// 2d map store in single dimensional array
int *map;

// key code
int keypress;

// game state
int current_state;

// 40x20 + 2 cells for bounds
int map_width = 42;
int map_height = 22;

int map_length;
int map_moves[4];

int max_players = 4;
int remaining_players = 0;

// pointers
LightCycle **ptr_players;
WINDOW *ptr_window;

int main(int argc, char **argv)
{
    create_window();
    create_menu(); 
    srand(time(NULL));
    tick();
    return 0;
}

void start()
{
    /* create_map(); */
    create_players();
    current_state = PLAY;
}

void tick()
{
    do {
        update();
        draw();
        usleep(FPS);
    } while (true);
} 

void create_map()
{
    map_length = map_width * map_height;
    map = malloc(map_length * sizeof(int));
    for (int i = 0; i < map_length; i++) {
        map[i] = EMPTY;
    }

    map_moves[0] = -1;
    map_moves[1] = -map_width;
    map_moves[2] = 1; 
    map_moves[3] = map_width;

    create_bound(0, 0, map_width - 1, map_height - 1);
} 

LightCycle *new_player(int x, int y, enum directions direction, int color)
{
    LightCycle *player = malloc(sizeof(LightCycle));
    player->position = x + y * map_width;
    player->direction = direction;
    player->color = color;
    player->alive = true;
    return player;
}

void update()
{
    keypress = getch();
    if (keypress == 'q' || keypress == 'Q') {
        quit();
    }

    if (current_state != PLAY) {
        if (keypress == KEY_ENTER || keypress == 10) {
            start();
        }
        return;
    }

    input_player_direction(ptr_players[0]);
    for (int curr_player_index = 0; curr_player_index < max_players; curr_player_index++) {

        // player dead
        if (!ptr_players[curr_player_index]->alive) continue;

        // update position
        if (curr_player_index > 0) {

            // get positions all players
            int *positions = malloc(max_players * sizeof(int));
            for (int position_index = 0; position_index < max_players; position_index++) {
                positions[position_index] = ptr_players[position_index]->position;
            }

            int new_position = (int long) maxn(positions, 6, curr_player_index, ptr_players[curr_player_index]->direction); 
            ptr_players[curr_player_index]->direction = new_position;
        }

        ptr_players[curr_player_index]->position += map_moves[ptr_players[curr_player_index]->direction];
        
        // add player to 2d map
        fulfill(ptr_players[curr_player_index]);
    }
} 

void draw()
{
    if (current_state == GAME_OVER) return;
    if (current_state == PLAY) erase();
    for (int i = 0; i < map_length; i++) {
        int x = i % map_width;
        int y = (int) i / map_width;
        draw_char(x, y, map[i]);
    }
    
    refresh();
}

int_array *floodfill(int curr_pos)
{
    int id = curr_pos;
    int dist = 1;

    int_array* q = int_array_new();
    int_array_add(q, id);

    int_array* q2 = int_array_new();

    int_array* distMap = int_array_new();
    int_array_set(distMap, curr_pos, dist);
    
    int safe_it = 20000;

    while(q->length) {

        break; //@FIXME
        if (safe_it-- < 0) break;

        dist++;

        int q_length = q->length;
        for (int i = 0; i < q_length; i++) {

            int _id = int_array_get(q, i);
            for (int move = 0; move < 4; move++) {

                int nextId = _id + map_moves[move];
                if (int_array_has(distMap, nextId) && map[nextId] != EMPTY) {
                    continue;
                }

                int_array_set(distMap, nextId, dist);
                int_array_add(q2, nextId);
            }
        }

        int_array_clear(q); 
        for (int i = 0; i < q2->length; i++) {
            int_array_add(q, int_array_get(q2, i));
        }
        int_array_clear(q2); 
    }

    return distMap;
}

int *evaluatePositions(int *positions)
{
    int *scores = malloc(max_players * sizeof(int*));
    int_array **distMaps = malloc(max_players * sizeof(int_array*));

    for (int i = 0; i < max_players; i++) {
        distMaps[i] = floodfill(positions[i]);
        scores[i] = 0;
    } 

    for (int i = 0; i < map_length; i++) {

        if (map[i] != EMPTY) {
            continue;
        }

        for (int curr_player_index = 0; curr_player_index < max_players; curr_player_index++) {

            for (int position_index = 0; position_index < max_players; position_index++) {

                if (curr_player_index == position_index) {
                    continue;
                }

                if (
                    int_array_get(distMaps[curr_player_index], i) != -1 &&  
                    int_array_get(distMaps[position_index], i) != -1
                ) {
                    int diff = int_array_get(distMaps[curr_player_index], i) - int_array_get(distMaps[position_index], i);
                    if (diff < 0) {
                        scores[curr_player_index] += 1;
                    }
                    continue;
                }

                if (int_array_get(distMaps[curr_player_index], i) != -1) {
                    scores[curr_player_index] += 2;
                }
            }

        }
    }

    return scores;
}

// https://project.dke.maastrichtuniversity.nl/games/files/phd/Nijssen_thesis.pdf
// http://web.cs.du.edu/~sturtevant/papers/multiplayergamesthesis.pdf
int *maxn(int *positions, int depth, int player_index, int best_move)
{
    if (depth == 0) return evaluatePositions(positions);

    int next_player_index = (player_index + 1) % max_players;
    int *best_scores = (int[4]) {99999, 99999, 99999, 99999};

    for (int move = 0; move < 4; move++) {

        // clone
        int *_positions = malloc(max_players * sizeof(int*));
        for (int i = 0; i < max_players; i++) {
            _positions[i] = positions[i];
        }

        _positions[player_index] += map_moves[move];

        if (map[_positions[player_index]] != EMPTY) {
            continue;
        }

        // just for evaluate
        map[_positions[player_index]] = WALL;
        int *scores = maxn(_positions, depth - 1, next_player_index, -10);
        map[_positions[player_index]] = EMPTY;

        if (scores[player_index] < 0) {
            scores[player_index] = 1;
        }

        if (scores[player_index] > best_scores[player_index]) {
            best_scores = scores;
            best_move = move;
        }

    }
    if (best_move != -10) return (void*)(long) best_move;
    return best_scores;
}

void create_players()
{
    ptr_players = malloc(sizeof(LightCycle) * max_players);
    remaining_players = max_players;

    // 2d int_array with x, y, direction, color
    int positions[4][4] = {
        // top left
        {1, 3, RIGHT, COLOR_BLUE},
        // bottom right
        {map_width - 2, map_height - 4, LEFT, COLOR_RED},
        // top right
        {map_width - 2, 3, LEFT, COLOR_GREEN},
        // bottom left
        {1, map_height - 4, RIGHT, COLOR_YELLOW}
    };

    for (int i = 0; i < max_players; i++) {
        ptr_players[i] = new_player(positions[i][0], positions[i][1], positions[i][2], positions[i][3]);
    }
}

void restore_window()
{
    // curses call to restore the original window and leave
    endwin(); 
}

void quit()
{
    restore_window();
    exit(0);
} 

void draw_char(int x, int y, int value)
{
    if (value != EMPTY) mvaddch(y, x, value); 
}

void input_player_direction(LightCycle *player)
{
    int *directions = get_allowed_directions(player);
    int direction = player->direction;
    if (keypress == KEY_LEFT) direction = LEFT;
    if (keypress == KEY_RIGHT) direction = RIGHT;
    if (keypress == KEY_UP) direction = UP;
    if (keypress == KEY_DOWN) direction = DOWN;
    if (direction == directions[0] || direction == directions[1])  player->direction = direction;
}

void fulfill(LightCycle *player)
{
    map[player->position] = WALL | COLOR_PAIR(player->color);
}

int *get_allowed_directions(LightCycle *player)
{
    int *directions = malloc(sizeof(int) * 2);
    switch(player->direction) {
        case UP:
        case DOWN:
            directions[0] = RIGHT;
            directions[1] = LEFT;
        break;
        case LEFT:
        case RIGHT:
            directions[0] = DOWN;
            directions[1] = UP;
        break;
    }
    return directions; 
}

void create_menu()
{
    // test {

    /* current_state = MENU; */
    /* create_map(); */
    /* int  curr_pos = 2+2*map_width; */
    /* restore_window(); */
    /* create_players(); */


    /*  */
    /* for (int times = 0; times < 10; times++) { */
    /*     printf(" - #%d \n", times); */
    /*  */
    /*     for (int i = 0; i < max_players; i++) { */
    /*  */
    /*         printf("   %d >>> %d -> %d -> %d = %d\n", ptr_players[i]->position, i, ptr_players[i]->direction, map_moves[ptr_players[i]->direction], ptr_players[i]->position + map_moves[ptr_players[i]->direction]); */
    /*         #<{(| ptr_players[i]->position += map_moves[ptr_players[i]->direction]; |)}># */
    /*     } */
    /* } */

    /* int *positions = malloc(max_players * sizeof(int)); */
    /*  */
    /* for (int i = 0; i < max_players; i++) { */
    /*     positions[i] = ptr_players[i]->position; */
    /* } */
    /*  */
    /* for (int i = 0; i < max_players; i++) { */
    /*     printf(" - %d => %d | %d/%d\n", i, positions[i], (int) positions[i] % map_width, (int)positions[i]/map_width); */
    /* } */

    /* for (int i = 0; i < max_players; i++) { */
    /*     ptr_players[i]->position = (long int) maxn(positions, 6, i, ptr_players[i]->position); */
    /* } */

    /* printf("\n\n");  */
    /* for (int i = 0; i < max_players; i++) { */
    /*     printf(" - %d => %d | %d/%d\n", i, positions[i], (int) positions[i] % max_players, (int)positions[i]/max_players); */
    /* } */
    /* int_array * distMap = floodfill(curr_pos); */
    /*  */
    /* for (int i = 0; i < distMap->length; i++) { */
    /*     if (int_array_get(distMap, i) != -1) { */
    /*         printf(" - %d => %d\n", i, (int)(long) int_array_get(distMap, i)); */
    /*     } */
    /* } */

    /* printf("curr_pos[%d]\n", curr_pos); */
    /* exit(0); */
    /*  */
    // } test

    




    current_state = MENU;
    create_map();
    draw();
    int center_x = map_width/2;
    mvprintw(1, center_x - 14, " _____   ___    ___    _  _ ");
    mvprintw(2, center_x - 14, "|_   _| | _ \\  / _ \\  | \\| |");
    mvprintw(3, center_x - 14, "  | |   |   / | (_) | | .` |");
    mvprintw(4, center_x - 14, "  |_|   |_|_\\  \\___/  |_|\\_|");
    mvprintw(8, center_x-14, "PRESS 'ENTER' TO START/RESTART"); 
    mvprintw(10, center_x-2, "OR"); 
    mvprintw(12, center_x-7, "'Q' TO QUIT"); 
}

void create_window()
{
    // initialize window
    ptr_window = initscr(); 

    int screen_height, screen_width;
    getmaxyx(ptr_window, screen_height, screen_width); 

    if (screen_height < map_height || screen_width < map_width) {
        restore_window();
        printf(" - you need resize terminal to: cols %d rows %d", map_width, map_height);
        printf("\n - current size: cols %d rows %d\n", screen_width, screen_height);
        exit(1);
    } 

    // set screen size
    wresize(ptr_window, map_height, map_width);

    // set no waiting for Enter key
    cbreak(); 

    // disable Echo
    noecho(); 

    // clear screen, send cursor to position (0,0)
    clear(); 

    // implement all changes since last refresh
    refresh(); 

    // don't display a cursor
    curs_set(false);

    // enable keyboard mapping
    keypad(stdscr, TRUE);

    //Immediately record keystroke & don't interpret control characters
    /* raw(); */

    // disable waiting for user input
    nodelay(stdscr, TRUE);

    // allow transparent color
    use_default_colors();
    assume_default_colors(-1,-1);
    start_color();

    // registre colors
    init_pair(COLOR_WHITE, COLOR_BLACK, COLOR_WHITE);
    init_pair(COLOR_BLUE, COLOR_BLUE, -1);
    init_pair(COLOR_RED, COLOR_RED, -1);
    init_pair(COLOR_GREEN, COLOR_GREEN, -1);
    init_pair(COLOR_YELLOW, COLOR_YELLOW, -1);
}

void game_over(bool player_loses)
{
    current_state = GAME_OVER;
    char *text = " YOU LOST ";
    if (!player_loses) {
        text = " YOU WON ";
    }
    int x = (map_width / 2) - strlen(text)/2;
    attron(COLOR_PAIR(COLOR_WHITE));
    mvprintw(0, x, "%s", text); 
    attroff(COLOR_PAIR(COLOR_WHITE));
}

void create_bound(int x, int y, int width, int height)
{
    for (int _y = y; _y < height; _y++) {
        map[ x + _y * map_width] = BOUND_LINE_VERTICAL; 
        map[ width + _y * map_width] = BOUND_LINE_VERTICAL; 
    }
    for (int _x = x; _x < width; _x++) {
       map[_x + 0 * map_width] = BOUND_LINE_HORIZONTAL;
       map[_x + height * map_width] = BOUND_LINE_HORIZONTAL;
    }
    map[x + y * map_width] = BOUND_CORNER;
    map[x + height * map_width] = BOUND_CORNER;
    map[width + y * map_width] = BOUND_CORNER;
    map[width + height * map_width] = BOUND_CORNER;
}
