//
// Created by tesserakt on 01.12.2021.
//

#pragma once

#include "widgets/Widget.h"

class BindableWidget : public virtual Widget {
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
            if (!next->template is<BindableWidget>()) continue;
            for (auto child : next->template as<BindableWidget>()->children) {
                if (!visitMap[child.get()]) {
                    action(child);
                    dfs.push(child.get());
                    visitMap[child.get()] = true;
                }
            }
        }
    }

protected:
    std::vector<std::shared_ptr<Widget>> children;

public:
    virtual void bind(std::shared_ptr<Widget> widget);

    template <typename T>
    auto getChildrenWithType() {
        std::vector<std::shared_ptr<T>> found;
        for (auto &child : children)
            if (child->template is<T>())
                found.push_back(child->template as<T>());
        return found;
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

    std::shared_ptr<Widget> getChildWithName(const std::string &name);

    const std::vector<std::shared_ptr<Widget>> &getChildren() const;
};