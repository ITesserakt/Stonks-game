//
// Created by tesserakt on 27.11.2021.
//

#include "game_widgets/Sale.h"

void Sale::show() {
    if (isHidden) return;

    move(position.y, position.x);
    if (isBlowing) {
        init_pair(widgetId, COLOR_BLACK, col);
        attron(COLOR_PAIR(widgetId));
        printw("%d. %s", index, name.c_str());
        attroff(COLOR_PAIR(widgetId));
        move(position.y + 1, position.x);
        if (object.has_value())
            printw("Sell for: $%lf", newPrice);
    } else {
        printw("%d. %s", index, name.c_str());
    }
}

GameObject::Cost Sale::currentPrice() {
    if (object.has_value() && newPrice == 0)
        newPrice = object->cost;
    return newPrice;
}

void Sale::growPrice(GameObject::Cost value) {
    if (object.has_value() && newPrice == 0)
        newPrice = object->cost;
    if (newPrice + value >= 0)
        newPrice += value;
}

UISize Sale::getSize() {
    UISize size;
    size.height = getHeight(name);
    size.height += isBlowing && object.has_value() ? 2 : 0;
    size.width = getWidth(name) + 4;
    return size;
}

void Sale::update(GameObject &&item) {
    object.emplace(item);
    if (newPrice == 0)
        newPrice = item.cost;
    if (dynamic_cast<UpdateCommand<Sale> *>(todo.get()) != nullptr)
        dynamic_cast<UpdateCommand<Sale> *>(todo.get())->update();
}
