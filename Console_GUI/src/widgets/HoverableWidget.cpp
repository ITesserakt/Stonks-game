//
// Created by tesserakt on 23.11.2021.
//

#include "widgets/HoverableWidget.h"
#include "widgets/BindableWidget.h"

void HoverableWidget::onHoverStart() {
    turnOn(COLOR_GREEN);

    if (is<BindableWidget>())
        for (auto x : as<BindableWidget>()->getChildrenRecursively<ColorWidget>())
            x->turnOn(COLOR_GREEN);
}

void HoverableWidget::onHoverEnd() {
    turnOff();

    if (is<BindableWidget>())
        for (auto x : as<BindableWidget>()->getChildrenRecursively<ColorWidget>())
            x->turnOff();
}

void HoverableWidget::click() {
    todo->act();
}

void HoverableWidget::setName(std::string newName) { name = std::move(newName); }

int HoverableWidget::getTabIndex() const { return tabIndex; }
