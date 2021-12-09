#pragma once

#include "StateCommand.h"
#include "Command.h"

struct ChangeActiveWidgetCommand : virtual StateCommand, virtual CloneCommand<ChangeActiveWidgetCommand>{
    void act() override {
        this->state.getCurrentScene().changeActiveWidget(direct, length);
    }

    ChangeActiveWidgetCommand(WorldState& state, Direction direct, unsigned int length = 1)
        : direct(direct), length(length), StateCommand(state) {}

    Direction direct;
    unsigned int length;
};