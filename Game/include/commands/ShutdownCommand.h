//
// Created by tesserakt on 27.11.2021.
//

#pragma once

#include "StateCommand.h"

struct ShutdownCommand : public virtual StateCommand, public virtual CloneCommand<ShutdownCommand> {
    explicit ShutdownCommand(WorldState &State) : StateCommand(State) {}

    void act() override {
        state.shutdown();
    }
};