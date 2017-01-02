#include <main.h>

int main()
{
    int width = 40;
    int height = 20;

    Board* board = board_create(width, height);
    Window* window = window_create(width, height);
    State* Menu = menu_create();
    Game* game = game_create(window, board, menu);

    game_start(game);

    return 0;
}
