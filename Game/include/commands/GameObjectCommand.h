//
// Created by tesserakt on 27.11.2021.
//

#pragma once

#include <optional>

#include "Command.h"
#include "GameObject.h"

class GameObjectCommand : public virtual Command {
protected:
    std::optional<GameObject> object;

public:
    GameObjectCommand() = default;

    explicit GameObjectCommand(GameObject object) : object(object) {}
};