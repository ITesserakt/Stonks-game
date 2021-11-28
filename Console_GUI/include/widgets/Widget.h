#pragma once

#include <map>
#include <memory>
#include <stack>
#include <string>
#include <utility>
#include <vector>

#include "utils.h"

class Widget : public std::enable_shared_from_this<Widget> {
private:
    std::map<Widget *, bool> visitMap;

    template <typename F>
    void dfs(F action) {
        for (auto &v : visitMap) {
            v.second = false;
        }
        std::stack<Widget *> dfs;
        dfs.push(this);
        while (!dfs.empty()) {
            auto next = dfs.top();
            dfs.pop();
            for (auto child : next->children) {
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
    std::vector<std::shared_ptr<Widget>> children;

public:
    Widget(const Widget &) = delete;

    Widget &operator=(const Widget &) = delete;

    Widget(Widget &&) = default;

    virtual ~Widget() = default;

    Widget &operator=(Widget &&) = default;

    explicit Widget(std::string name);

    virtual void show() {
        for (auto child : children)
            child->show();
    }

    virtual void bind(std::shared_ptr<Widget> widget);

    template <typename T>
    bool is() { return dynamic_cast<T *>(this); }

    template <typename T>
    std::shared_ptr<T> as() {
        if (!is<T>())
            throw std::runtime_error("Bad cast");
        return std::dynamic_pointer_cast<T>(shared_from_this());
    }

    template <typename T>
    auto getChildrenWithType() {
        std::vector<std::shared_ptr<T>> found;
        for (auto &child : children)
            if (child->template is<T>())
                found.push_back(child->template as<T>());
        return found;
    }

    auto getChildWithName(const std::string &name) {
        auto result = std::find_if(children.begin(), children.end(), [&](auto a) {
            return a->name == name;
        });
        if (result != children.end())
            return *result;
        throw std::runtime_error("Could not find child");
    }

    template <typename T>
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

    const std::vector<std::shared_ptr<Widget>> &getChildren() const;

    friend std::ostream &operator<<(std::ostream &os, const Widget &widget);

    bool operator==(const Widget &rhs) const;

    bool operator!=(const Widget &rhs) const;
};