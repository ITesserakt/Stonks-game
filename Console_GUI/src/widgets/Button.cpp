#include "widgets/Button.h"
#include "utils.h"
#include "events/ChangeCanvas.h"
#include <stdlib.h>
#include <ncurses.h>

Button::Button(std::string Name) : Widget(Name) {
    isBlowing = false;
    isClicable = false;
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
    isClicable = false;
    return std::unique_ptr<GUIEvent>();
}

std::unique_ptr<GUIEvent> Button::onHoverStart() {
    turnOn();
    isClicable = true;
    return std::unique_ptr<GUIEvent>();
}
