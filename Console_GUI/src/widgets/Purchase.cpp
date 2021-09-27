#include "widgets/Purchase.h"

UISize Purchase::getSize() {
    return UISize();
}

void Purchase::show() {
    if (isHidden) { return; }

    move(position.y, position.x);
    if (isBlowing) {
        init_pair(widgetId, COLOR_BLACK, col);
        attron(COLOR_PAIR(widgetId));
        printw("%s", name.c_str());
        attroff(COLOR_PAIR(widgetId));
    }
    else {
        printw("%s", name.c_str());
    }
}

Purchase::Purchase(int index) : Widget("Purchase"), HoverableWidget(index) {
    size.height = getHeight(name);
    size.width = getWidth(name);
}

void Purchase::click() {

}
