//
// Created by tesserakt on 01.12.2021.
//

#include "widgets/BindableWidget.h"

void BindableWidget::bind(std::shared_ptr<Widget> widget) {
    if (this == widget.get()) { throw std::runtime_error("Cannot add itself as a child"); }

    widget->parent = weak_from_this();
    children.push_back(widget);
    visitMap[widget.get()] = false;
}

std::shared_ptr<Widget> BindableWidget::getChildWithName(const std::string &name) {
    auto result = std::find_if(children.begin(), children.end(), [&](auto a) { return a->name == name; });

    if (result != children.end()) return *result;

    for (auto &child : children)
        if (child->is<BindableWidget>()) {
            try {
                return child->as<BindableWidget>()->getChildWithName(name);
            } catch (...) { continue; }
        }
    throw std::runtime_error("Cannot find widget with name " + name + " in " + getName());
}

const std::vector<std::shared_ptr<Widget>> &BindableWidget::getChildren() const {
    return children;
}
