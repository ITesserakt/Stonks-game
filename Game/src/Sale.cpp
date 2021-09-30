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
    }
    else {
        printw("%d. %s", tabIndex + 1, name.c_str());
    }
}

UISize Sale::getSize() {
    size.height = getHeight(name);
    size.height += this->isBlowing ? 2 : 0;
    size.width = getWidth(name) + 4;
    return size;
}

