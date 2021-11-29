//
// Created by tesserakt on 27.11.2021.
//

#pragma once

#include <utility>

#include "StateCommand.h"

class SceneChangeCommand : public virtual StateCommand, public virtual CloneCommand<SceneChangeCommand> {
private:
    std::shared_ptr<Canvas> to;

public:
    SceneChangeCommand(WorldState &state, std::shared_ptr<Canvas> changeTo)
        : StateCommand(state), to(std::move(changeTo)) {}

    void act() override {
        state.changeCurrentScene(*to);
    }
};