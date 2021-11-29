#include "widgets/Button.h"

void Button::show() {
    if (isHidden) return;

    if (isBlowing) { init_pair(widgetId, COLOR_BLACK, col); }
    if (size.height == 1) {
        if (isBlowing) { attron(COLOR_PAIR(widgetId)); }
        move(position.y, position.x);
        printw("[ %s ]", name.c_str());
        if (isBlowing) { attroff(COLOR_PAIR(widgetId)); }
    } else {
        if (isBlowing) { attron(COLOR_PAIR(widgetId)); }
        for (int i = 0; i < getHeight(name); i++) {
            move(position.y + i, position.x);
            printw("| %s |", splitBySentence(name, i).c_str());
        }
        if (isBlowing) { attroff(COLOR_PAIR(widgetId)); }
    }

    return Widget::show();
}

UISize Button::getSize() {
    if (isHidden)
        return {0, 0};
    size = {getWidth(name), getHeight(name)};
    return size;
}

Button::Button(std::string name, int index) : Widget(std::move(name)),
                                              ColorWidget(name),
                                              HoverableWidget(index, Command::noop()) {
    isBlowing = false;
    size = {getWidth(name), getHeight(name)};
}
