#include "widgets/Button.h"
#include "Utils.h"
#include <ncurses.h>

Button::Button(std::string Name) : Widget(Name) {
    isBlowing = false;
    height = getHeight(Name);
    width = getWidth(Name);
}

void Button::show() {
    if (height == 1)
        printw("[ %s ]", name.c_str());
    // else big button
}

Rect<unsigned int> Button::getSize() {
    return Rect(Point<unsigned>(0,0), Point<unsigned>(width, height));
}

EventSequence Button::onHoverEnd() {}
EventSequence Button::onHoverStart() {}
EventSequence Button::click() {}
