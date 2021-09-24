#include "thread"
#include "Frontend.h"
#include "widgets/Button.h"
#include <ncurses.h>

int main() {
    initscr();
    start_color();
    noecho();                       // Removes characters when typed
    curs_set(0);                    // Removes cursor
	//auto f = Frontend();
    getch();
    endwin();
}