#include "widgets/Button.h"
#include "utils.h"
#include <cstdlib>
#include <iostream>

void Button::show() {
    if (isHidden) return;

    move(position.y, position.x);
    if (isBlowing) { init_pair(widgetId, COLOR_BLACK, col); }
    if (size.height == 1) {
        if (isBlowing) { attron(COLOR_PAIR(widgetId)); }
        printw("[ %s ]", name.c_str());
        if (isBlowing) { attroff(COLOR_PAIR(widgetId)); }
    }
}

void Button::click() {
    if (destiny == Quiter) {
        endwin();
        std::cout << "See you later" << std::endl;
        exit(0);
    } else
        return;
}