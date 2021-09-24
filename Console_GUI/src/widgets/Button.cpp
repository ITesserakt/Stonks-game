#include "widgets/Button.h"
#include "utils.h"
#include <ncurses.h>

Button::Button(std::string Name) : Widget(Name) {
    isBlowing = false;
    height = getHeight(Name);
    width = getWidth(Name);
}

void Button::show() {
    if (isBlowing) { init_pair(1, COLOR_BLACK, col); }
    if (height == 1) {
        if (isBlowing) { attron(COLOR_PAIR(1)); }
        printw("[ %s ]", name.c_str());
        if (isBlowing) { attroff(COLOR_PAIR(1)); }
    }
    // else big button
}

Rect<unsigned int> Button::getSize() {
    return Rect(Point<unsigned>(0,0), Point<unsigned>(width, height));
}

EventSequence Button::onHoverEnd() {}
EventSequence Button::onHoverStart() {}
EventSequence Button::click() {}
