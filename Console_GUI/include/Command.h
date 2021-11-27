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

class WidgetCommand : public Command {
protected:
    HoverableWidget &sender;

public:
    explicit WidgetCommand(HoverableWidget &sender) : sender(sender) {}

    template <typename T, typename F>
    auto forward(F &&newFn) && {
        struct DecoratingCommand : WidgetCommand {
            F f;

            explicit DecoratingCommand(WidgetCommand &&inner, F &&f)
                : f(std::forward<F>(f)), WidgetCommand(inner) {}

            void act() override {
                return f(*this->sender.template as<T>());
            }
        };

        return DecoratingCommand(std::move(*this), std::forward<F>(newFn));
    }

    template <typename T = HoverableWidget, typename F>
    static auto fromFunction(T &sender, F &&fn) {
        struct FnCommand : WidgetCommand {
            F f;
            T &refinedSender;

            explicit FnCommand(T &sender_, F &&f)
                : f(std::forward<F>(f)), refinedSender(sender_), WidgetCommand(sender_) {}

            void act() override {
                return f(this->refinedSender);
            }
        };

        return FnCommand(sender, std::forward<F>(fn));
    }
};