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

const std::vector<std::shared_ptr<Widget>> &Widget::getChildren() const { return children; }

std::shared_ptr<Canvas> Widget::getCanvas() const { return canvas; }

void Widget::bind(std::shared_ptr<Widget> widget) {
    if (this == widget.get()) {
        throw std::runtime_error("Cannot add itself as a child");
    }

    widget->parent = weak_from_this();
    widget->canvas = canvas;
    children.push_back(widget);
    visitMap[widget.get()] = false;
}

Widget::Widget(std::string name)
        : widgetId(generateId()), name(std::move(name)) {}
