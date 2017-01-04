
src/
    core/
        board.c
        game.c
        window.c
        negamax.c
        input.c
        player.c
        ai.c
    state/
        menu.c
        play.c
        win.c
    common/
        array.c
        int_array.c
        ptr_array.c
        test.c
    main.c



roadmap
    version 1.0 (current)
        + ncurses
        + player vs 1 AI(negamax)
    version 2.0
        - ncurses
        - negamax
        + add SDL
        + add +2 AI
        + maxn (negamax for more than two players)
    version 3.0
        + effects
        + sounds


main.c
    globals
        Game* game

game.c
    Game {
        Player* players;
        Board* board;
        Window* window;
        Menu* menu;
        int state;
    }
    game_create()
        create_window()
        create_board()
        create_menu()
        create_players()
    game_tick()
    game_update()
    game_draw()


menu.c
    menu_create()


window.c
    window_create()
    window_draw()
    window_quit()


board.c
    board_create()
    board_allowed_directions()


player.c
    player_create()


negamax.c
    negamax()
