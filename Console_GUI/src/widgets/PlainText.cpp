#include "widgets/PlainText.h"

PlainText::PlainText(std::string name) : Widget(name) {
    size.width = getWidth(name);
    size.height = getHeight(name);
}

void PlainText::show() {
    if (isHidden) return;

    move(position.y, position.x);
    if (isBlowing) { init_pair(widgetId, COLOR_BLACK, col); }
    if (isBlowing) { attron(COLOR_PAIR(widgetId)); }
    printw("%s", name.c_str());
    if (isBlowing) { attroff(COLOR_PAIR(widgetId)); }
}
