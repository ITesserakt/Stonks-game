#include "Canvas.h"
#include <ncurses.h>

void Canvas::show() {
    int y = 0, x = 0;

    if (align == Left) {
        for (const auto &child: children) {
            move(y, x);
            child->show();
            y++;
        }
    }
    else if (align == Centered) {

    }
    else if (align == Right) {
        x = getmaxx(initscr());
        for (const auto &child: children) {
            move(y, x - child->getSize().width);
            child->show();
            y++;
        }
    }
}

Size<unsigned int> Canvas::getSize() {
    Size<unsigned> sizeOfWindow;
    sizeOfWindow.width  = getmaxx(initscr());
    sizeOfWindow.height = getmaxy(initscr());
    return sizeOfWindow;
}
