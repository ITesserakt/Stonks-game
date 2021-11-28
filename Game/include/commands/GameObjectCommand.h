//
// Created by tesserakt on 27.11.2021.
//

#pragma once

#include "optional.hpp"
#include "Command.h"
#include "GameObject.h"

class GameObjectCommand : public virtual Command {
protected:
    std::experimental::optional<GameObject> object;

public:
    GameObjectCommand() = default;
};