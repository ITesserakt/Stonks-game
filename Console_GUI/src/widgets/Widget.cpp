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

void Widget::bind(std::shared_ptr<Widget> widget) {
    if (this == widget.get()) {
        throw std::runtime_error("Cannot add itself as a child");
    }

    //widget->parent = weak_from_this();
    widget->parent = std::weak_ptr<Widget>(shared_from_this());
    children.push_back(widget);
    visitMap[widget.get()] = false;
}

Widget::Widget(std::string name)
    : widgetId(generateId()), name(std::move(name)) {}

bool Widget::operator==(const Widget &rhs) const {
    return widgetId == rhs.widgetId;
}

bool Widget::operator!=(const Widget &rhs) const {
    return !(rhs == *this);
}

void Widget::show() {
    for (auto child : children)
        child->show();
}

std::shared_ptr<Widget> Widget::getChildWithName(const std::string &name) {
    auto result = std::find_if(children.begin(), children.end(), [&](auto a) {
        return a->name == name;
    });
    if (result != children.end())
        return *result;
    throw std::runtime_error("Could not find child");
}
