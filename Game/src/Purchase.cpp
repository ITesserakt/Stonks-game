#include <Event.h>
#include "Purchase.h"
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
    if (isBlowing && cost != 0) {
        move(position.y + 1, position.x);
        printw("Cost: $%lf", cost);
    }
}

Purchase::Purchase(int index, WorldState& state, std::function<void(WorldState& s, Purchase&)> f) :
    Widget(""),
    ColorWidget(""),
    HoverableWidget(index, state, [f](WorldState& s, HoverableWidget& w) {
        f(s, *w.as<Purchase>());
    }) {}
