#include "test.h"
#include "window.h"

#include <ncurses.h>

Board* board = NULL;
Window* window = NULL;

static bool window_has_string(int x, int y, char* string)
{
    char buf[1024];
    mvinnstr(y, x, buf, strlen(string));
    return strcmp(string, buf) == 0;
}

void test_draw()
{
    window_draw_string(0, 0, "test draw string");
    window_draw_char(5, 5, 'a');
    window_refresh();

    ASSERT(window_has_string(0, 0, "test draw"));
    ASSERT(window_has_string(10, 0, "string"));
    ASSERT(window_has_string(5, 5, "a"));
}

void test_sizes()
{
    ASSERT(window->width >= board->width);
    ASSERT(window->height >= board->height);
}

void tearup()
{
    board = board_create();
    window = window_create(board);
}

void teardown()
{
    window_restore();
}

int main(int argc, char* argv[]) 
{
    Test_describe("window()");

    Test_tearup(tearup);
    Test_teardown(teardown);

    Test_it("should fit in screen", test_sizes);
    Test_it("should draw", test_draw);

    return Test_run();
}
