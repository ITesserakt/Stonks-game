#pragma once

#include "utils.h"
#include <utility>
#include <vector>
#include <string>
#include <memory>
#include <iostream>

class Canvas;

class Widget : public std::enable_shared_from_this<Widget> {
    // TODO: Base class to represent widget, i.e. text label, button, etc
protected:
    std::string name;
    std::weak_ptr<Widget> parent;
    std::shared_ptr<Canvas> canvas;
    std::vector<std::shared_ptr<Widget>> children;

public:
    Widget(const Widget &) = delete;

    Widget &operator=(const Widget &) = delete;

    Widget(Widget &&) = default;

    Widget &operator=(Widget &&) = default;

    explicit Widget(std::string name) : name(std::move(name)) {}

    virtual void show() = 0;

    template<typename T>
    void bind(T &&widget) {
        if (this == &widget)
            throw std::runtime_error("Cannot add itself as a child");

        widget.parent = weak_from_this();
        widget.canvas = canvas;
        children.push_back(std::make_shared<T>(std::forward<T>(widget)));
    }

    const std::string &getName() const { return name; }

    std::weak_ptr<Widget> getParent() const { return parent; }

    std::vector<std::shared_ptr<Widget>> getChildren() const { return children; }

    std::shared_ptr<Canvas> getCanvas() const { return canvas; }
};