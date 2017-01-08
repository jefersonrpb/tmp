#include "state/menu.h"

static void update(Game* game);
static void draw(Game* game);

State* menu_create()
{
    State* menu = malloc(sizeof(State));
    menu->update = update;
    menu->draw = draw;
    return menu;
}

void menu_destroy(State* menu)
{
    free(menu);
}

static void update(Game* game)
{

}

static void draw(Game* game)
{

}
