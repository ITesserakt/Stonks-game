//
// Created by Сева on 23.09.2021.
//

#include "widgets/Widget.h"

std::ostream &operator<<(std::ostream &os, const Widget &widget) {
    os << "Widget {" << widget.name << "}";
    return os;
}

const std::string &Widget::getName() const { return name; }

std::weak_ptr<Widget> Widget::getParent() const { return parent; }

Widget::Widget(std::string name)
    : widgetId(generateId()), name(std::move(name)) {}

bool Widget::operator==(const Widget &rhs) const {
    return widgetId == rhs.widgetId;
}

bool Widget::operator!=(const Widget &rhs) const {
    return !(rhs == *this);
}