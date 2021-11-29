//
// Created by tesserakt on 23.11.2021.
//

#include "widgets/HoverableWidget.h"

void HoverableWidget::onHoverStart() {
    turnOn(COLOR_GREEN);

    for (auto x : getChildrenRecursively<ColorWidget>())
        x->turnOn(COLOR_GREEN);
}

void HoverableWidget::onHoverEnd() {
    turnOff();

    for (auto x : getChildrenRecursively<ColorWidget>())
        x->turnOff();
}

void HoverableWidget::click() {
    todo->act();
}

void HoverableWidget::setName(std::string newName) { name = std::move(newName); }

int HoverableWidget::getTabIndex() const { return tabIndex; }
