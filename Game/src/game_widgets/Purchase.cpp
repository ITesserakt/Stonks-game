//
// Created by tesserakt on 27.11.2021.
//

#include "game_widgets/Purchase.h"

void Purchase::show() {
    if (isHidden) return;

    move(position.y, position.x);
    if (isBlowing) {
        init_pair(widgetId, COLOR_BLACK, col);
        attron(COLOR_PAIR(widgetId));
        printw("%d. %s", tabIndex + 1, name.c_str());
        attroff(COLOR_PAIR(widgetId));
    } else {
        printw("%d. %s", tabIndex + 1, name.c_str());
    }
    if (isBlowing && object.has_value()) {
        move(position.y + 1, position.x);
        printw("Cost: $%lf", object->cost);
    }
}

void Purchase::update(GameObject &&item) {
    object.emplace(item);
    if (dynamic_cast<UpdateCommand<Purchase> *>(todo.get()) != nullptr)
        dynamic_cast<UpdateCommand<Purchase> *>(todo.get())->update();
}

UISize Purchase::getSize() {
    UISize size;
    size.height = getHeight(name);
    size.height += isBlowing && object.has_value() ? 2 : 0;
    size.width = getWidth(name) + 4;
    return size;
}

