//
// Created by tesserakt on 21.11.2021.
//

#pragma once

#include <memory>
#include <utility>

class HoverableWidget;
class WorldState;

class Command {
protected:
    WorldState &state;
    std::shared_ptr<HoverableWidget> sender;

public:
    Command(WorldState &state, std::shared_ptr<HoverableWidget> sender) : sender(std::move(sender)), state(state) {}

    virtual void act() = 0;

    ~Command() = default;

    template <typename F>
    static auto fromFunction(WorldState &state, std::shared_ptr<HoverableWidget> sender, F fn) {
        struct FnCommand : Command {
            F f;

            explicit FnCommand(F f) : f(f), Command(state, sender) {}

            void act() override {
                return f(this->state, *this->sender);
            }
        };

        return std::make_unique<FnCommand>(fn);
    }

    template <typename F>
    static auto forward(Command &&old, F newFn) {
        struct DecoratingCommand : Command {
            F f;

            explicit DecoratingCommand(Command &&inner, F f)
                : f(f), Command(std::move(inner)) {}

            void act() override {
                return f(this->state, *this->sender);
            }
        };

        return std::make_unique<DecoratingCommand>(std::move(old), newFn);
    }
};