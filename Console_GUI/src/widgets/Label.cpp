#include "widgets/Label.h"

Label::Label(std::string name, const std::string& text) : Widget(name), text(text) {
    size.width = getWidth(text);
    size.height = getHeight(text);
}

void Label::show() {
    if (isHidden) return;

    move(position.y, position.x);
    if (isBlowing) { init_pair(widgetId, COLOR_BLACK, col); }
    if (isBlowing) { attron(COLOR_PAIR(widgetId)); }
    printw("%s\n", text.c_str());
    if (isBlowing) { attroff(COLOR_PAIR(widgetId)); }
}
