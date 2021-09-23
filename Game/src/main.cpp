#include "thread"
#include "Frontend.h"
#include "widgets/Button.h"
#include <ncurses.h>

int main() {
    initscr();
	//auto f = Frontend();
    Button but("Hello");
    but.show();
    getch();
    //endwin();
}