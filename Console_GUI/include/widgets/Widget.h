#pragma once

#include "utils.h"
#include <utility>
#include <vector>
#include <string>
#include <memory>
#include <iostream>

class Canvas;

class Widget : public std::enable_shared_from_this<Widget> {
protected:
    int widgetId;
    bool isHidden;

    std::string name;
    std::weak_ptr<Widget> parent;
    std::shared_ptr<Canvas> canvas;
    std::vector<std::shared_ptr<Widget>> children;

public:
    Widget(const Widget &) = delete;
    Widget &operator=(const Widget &) = delete;
    Widget(Widget &&) = default;
    Widget &operator=(Widget &&) = default;

    explicit Widget(std::string name)
    : name(std::move(name)), widgetId(generateId()) {}

    virtual void show() = 0;
    virtual void hide(bool hide) { this->isHidden = hide; }

    virtual void changePos(int x, int y) = 0;
    virtual Size<unsigned> getSize() = 0;
    virtual bool isClickable() { return false; }

    void bind(const std::shared_ptr<Widget>& widget) {
        if (this == widget.get()) {
            throw std::runtime_error("Cannot add itself as a child");
        }

        widget->parent = weak_from_this();
        widget->canvas = canvas;
        children.push_back(widget);
    }

    const std::string &getName() const { return name; }

    std::weak_ptr<Widget> getParent() const { return parent; }

    std::vector<std::shared_ptr<Widget>> getChildren() const { return children; }

    std::shared_ptr<Canvas> getCanvas() const { return canvas; }
};