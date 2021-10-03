#include "GameWidgets/Purchase.h"
#include "Canvas.h"
#include <utility>

Purchase::Purchase(int index, WorldState& state, std::function<void(WorldState& s, Purchase&)> f) :
        Widget(""),
        ColorWidget(""),
        HoverableWidget(index, state, [f](WorldState& s, HoverableWidget& w) {
            f(s, *w.as<Purchase>());
        }) {}

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

UISize Purchase::getSize() {
    return findSize(this);
}