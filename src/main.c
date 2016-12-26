#include <main.h>

int main()
{
    int width = 40;
    int height = 20;

    Board* board = board_create(width, height);
    Window* window = window_create(width, height);
    Game* game = game_create(window, board);

    game_start(game);

    return 0;
}
