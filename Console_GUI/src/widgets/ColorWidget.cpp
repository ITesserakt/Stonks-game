//
// Created by tesserakt on 29.11.2021.
//

#include "widgets/ColorWidget.h"

void ColorWidget::turnOn(int color) {
    col = color;
    isBlowing = true;

    for (auto child : getChildrenWithType<ColorWidget>())
        child->turnOn(color);
}

void ColorWidget::turnOff() {
    isBlowing = false;

    for (auto child : getChildrenWithType<ColorWidget>())
        child->turnOff();
}

ColorWidget::ColorWidget(const std::string &name) : Widget(name) {}
