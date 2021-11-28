//
// Created by tesserakt on 21.11.2021.
//

#pragma once

#include <memory>
#include <utility>

#include "widgets/HoverableWidget.h"

class Command {
public:
    virtual void act() = 0;

    virtual ~Command() = default;

    static auto noop() {
        struct NoOpCommand : Command {
            void act() override {}
        };

        return NoOpCommand();
    }

    template <typename F>
    static auto fromFunction(F &&fn) {
        struct FnCommand : Command {
            F f;

            explicit FnCommand(F &&f) : f(std::forward<F>(f)) {}

            void act() override {
                return f();
            }
        };

        return FnCommand(std::forward<F>(fn));
    }
};

template <typename T>
class WidgetCommand : public virtual Command {
protected:
    T &sender;

public:
    explicit WidgetCommand(T &sender) : sender(sender) {}
};

template <typename T>
struct UpdateCommand : public virtual WidgetCommand<T> {
    virtual void update() = 0;
};