#include <Sale.h>

#include <utility>

Sale::Sale(int index, std::function<void(HoverableWidget &)> f) :
    Widget(""),ColorWidget(""),
    HoverableWidget(index, std::move(f))
{}

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
    size.width = getWidth(name) + 4;
    return size;
}

