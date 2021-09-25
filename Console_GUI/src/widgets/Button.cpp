#include "widgets/Button.h"
#include "utils.h"
#include <ncurses.h>

Button::Button(std::string Name) : Widget(Name) {
    isBlowing = false;
    size.height = getHeight(Name);
    size.width = getWidth(Name) + 4;
}

void Button::show() {
    if (isHidden) return;

    move(position.y, position.x);
    if (isBlowing) { init_pair(widgetId, COLOR_BLACK, col); }
    if (size.height == 1) {
        if (isBlowing) { attron(COLOR_PAIR(widgetId)); }
        printw("[ %s ]", name.c_str());
        if (isBlowing) { attroff(COLOR_PAIR(widgetId)); }
    }
    // else big button
}

EventSequence Button::onHoverEnd() {}
EventSequence Button::onHoverStart() {}
EventSequence Button::click() {}
