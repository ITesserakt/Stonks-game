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
            move(y, x - child->getSize().b.x);
            child->show();
            y++;
        }
    }
}

Rect<unsigned int> Canvas::getSize() {
    return Rect<unsigned int>();
}
