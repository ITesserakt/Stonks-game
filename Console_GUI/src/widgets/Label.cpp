#include "widgets/Label.h"

Label::Label(const std::string &name, const std::string &text)
    : Widget(name),
      ColorWidget(name),
      text(text) {
    size.width = getWidth(text);
    size.height = getHeight(text);
    start = std::chrono::system_clock::now();
}

void Label::show() {
    if (isHidden) return;

    if (std::chrono::system_clock::now() - start > delta && autoUpdateMode) {
        start = std::chrono::system_clock::now();
        text = newName();
    }

    move(position.y, position.x);
    if (isBlowing) { init_pair(widgetId, COLOR_BLACK, col); }
    if (isBlowing) { attron(COLOR_PAIR(widgetId)); }
    printw("%s\n", text.c_str());
    if (isBlowing) { attroff(COLOR_PAIR(widgetId)); }

    Widget::show();
}

void Label::setRegularNameChanging(std::chrono::milliseconds timeDelta, std::function<std::string()> rename) {
    autoUpdateMode = true;
    this->delta = timeDelta;
    this->newName = rename;
}
