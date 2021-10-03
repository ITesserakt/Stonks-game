#include <gameWidgets/Sale.h>

Sale::Sale(int index, WorldState& state, std::function<void(WorldState &,
                                                            Sale &)> f) :
    Widget(""),ColorWidget(""),
    HoverableWidget(index, state, [f](WorldState& s, HoverableWidget& w) {
        f(s, *w.as<Sale>());
    }) {}

void Sale::show() {
    move(position.y, position.x);
    if (isBlowing) {
        init_pair(widgetId, COLOR_BLACK, col);
        attron(COLOR_PAIR(widgetId));
        printw("%d. %s", tabIndex + 1, name.c_str());
        attroff(COLOR_PAIR(widgetId));
        move(position.y + 1, position.x);
        if (getItemId() != -1)
            printw("Sell for: $%lf", newPrice);
    }
    else {
        printw("%d. %s", tabIndex + 1, name.c_str());
    }
}

UISize Sale::getSize() {
    return findSize(this);
}

