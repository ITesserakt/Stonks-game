//
// Created by tesserakt on 29.11.2021.
//

#include "widgets/ColorWidget.h"
#include "widgets/BindableWidget.h"

void ColorWidget::turnOn(int color) {
    col = color;
    isBlowing = true;

    if (is<BindableWidget>())
        for (auto child : as<BindableWidget>()->getChildrenWithType<ColorWidget>())
            child->turnOn(color);
}

void ColorWidget::turnOff() {
    isBlowing = false;

    if (is<BindableWidget>())
        for (auto child : as<BindableWidget>()->getChildrenWithType<ColorWidget>())
            child->turnOff();
}

ColorWidget::ColorWidget(const std::string &name) : Widget(name) {}
