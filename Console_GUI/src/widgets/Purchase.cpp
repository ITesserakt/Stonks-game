#include <Event.h>
#include "widgets/Purchase.h"
#include "Canvas.h"
#include "Event.h"

#include <utility>

UISize Purchase::getSize() {
    size.height = this->getCanvas()->getActiveWidget().get() == this ?
            getHeight(name) + 2 : getHeight(name);
    size.width = getWidth(name) + 4;
    return size;
}

void Purchase::show() {
    if (isHidden) { return; }

    move(position.y, position.x);
    if (isBlowing) {
        init_pair(widgetId, COLOR_BLACK, col);
        attron(COLOR_PAIR(widgetId));
        printw("%d. %s", tabIndex + 1, name.c_str());
        attroff(COLOR_PAIR(widgetId));
    }
    else {
        printw("%d. %s", tabIndex + 1, name.c_str());
    }
}

Purchase::Purchase(int index, std::function<void(Purchase&)> f) :
    Widget(""),
    ColorWidget(""),
    HoverableWidget(index),
    todo(std::move(f)) {}

void Purchase::click() {
    todo(*this);
}
