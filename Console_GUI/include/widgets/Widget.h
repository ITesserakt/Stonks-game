#pragma once

#include <map>
#include <memory>
#include <stack>
#include <string>
#include <utility>
#include <vector>

#include "utils.h"

class Widget : public std::enable_shared_from_this<Widget> {
    friend class BindableWidget;

protected:
    int widgetId;
    std::string name;
    std::weak_ptr<Widget> parent;

public:
    Widget(const Widget &) = delete;

    Widget &operator=(const Widget &) = delete;

    Widget(Widget &&) = default;

    Widget &operator=(Widget &&) = default;

    virtual ~Widget() = default;

    explicit Widget(std::string name);

    virtual void show() = 0;

    template <typename T>
    bool is() {
        return dynamic_cast<T *>(this);
    }

    template <typename T>
    std::shared_ptr<T> as() {
        if (!is<T>())
            throw std::runtime_error("Bad cast");
        return std::dynamic_pointer_cast<T>(shared_from_this());
    }

    const std::string &getName() const;

    std::weak_ptr<Widget> getParent() const;

    friend std::ostream &operator<<(std::ostream &os, const Widget &widget);

    bool operator==(const Widget &rhs) const;

    bool operator!=(const Widget &rhs) const;
};