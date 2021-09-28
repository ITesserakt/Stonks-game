#pragma once

#include "utils.h"
#include <utility>
#include <map>
#include <stack>
#include <vector>
#include <string>
#include <memory>

class Canvas;

class Widget : public std::enable_shared_from_this<Widget> {
private:
    std::map<Widget *, bool> visitMap;

    template<typename F>
    void dfs(F action) {
        for (auto &v: visitMap | ranges::views::values) {
            v = false;
        }
        std::stack<Widget *> dfs;
        dfs.push(this);
        while (!dfs.empty()) {
            auto next = dfs.top();
            dfs.pop();
            for (auto child: next->children) {
                if (!visitMap[child.get()]) {
                    action(child);
                    dfs.push(child.get());
                    visitMap[child.get()] = true;
                }
            }
        }
    }

protected:
    int widgetId;

    std::string name;
    std::weak_ptr<Widget> parent;
    std::shared_ptr<Canvas> canvas;
    std::vector<std::shared_ptr<Widget>> children;

public:
    Widget(const Widget &) = delete;

    Widget &operator=(const Widget &) = delete;

    Widget(Widget &&) = default;
    virtual ~Widget() = default;

    Widget &operator=(Widget &&) = default;

    explicit Widget(std::string name);

    virtual void show() = 0;

    virtual void bind(std::shared_ptr<Widget> widget);

    template<typename T>
    bool is() { return dynamic_cast<T *>(this); }

    template<typename T>
    std::shared_ptr<T> as() {
        return std::dynamic_pointer_cast<T>(shared_from_this());
    }

    template<typename T>
    auto getChildrenWithType() {
        return children |
               ranges::views::filter(&Widget::is<T>) |
               ranges::views::transform(&Widget::as<T>);
    }

    template<typename T>
    auto getChildrenRecursively() {
        std::vector<std::shared_ptr<T>> result;
        dfs([&result](auto x) {
            if (x->template is<T>())
                result.push_back(x->template as<T>());
        });
        return result;
    }

    const std::string &getName() const;

    std::weak_ptr<Widget> getParent() const;

    const std::vector<std::shared_ptr<Widget>>& getChildren() const;

    std::shared_ptr<Canvas> getCanvas() const;

    friend std::ostream &operator<<(std::ostream &os, const Widget &widget);
};