#include "widgets/Button.h"
#include "utils.h"
#include <cstdlib>

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

std::unique_ptr<GUIEvent> Button::click() {
    if (destiny == CanvasChanger)
        return std::unique_ptr<GUIEvent>();
    else if (destiny == Quiter) {
        endwin();
        exit(0);
    }
    else
        return nullptr;
}

std::unique_ptr<GUIEvent> Button::onHoverEnd() {
    turnOff();
    return std::unique_ptr<GUIEvent>();
}

std::unique_ptr<GUIEvent> Button::onHoverStart() {
    turnOn();
    return std::unique_ptr<GUIEvent>();
}
