//
// Created by tesserakt on 27.11.2021.
//

#pragma once

#include "Command.h"
#include "WorldState.h"

class StateCommand : public virtual Command {
protected:
    WorldState &state;

public:
    explicit StateCommand(WorldState &state) : state(state) {}

    template <typename F>
    static auto fromFunction(WorldState &state, F &&fn) {
        struct FnCommand : virtual StateCommand, virtual CloneCommand<FnCommand> {
            F f;

            FnCommand(WorldState &State, F &&f) : StateCommand(State), f(f) {}

            void act() override {
                return f(state);
            }
        };

        return FnCommand(state, std::forward<F>(fn));
    }
};