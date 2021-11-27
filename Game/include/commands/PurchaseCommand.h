//
// Created by tesserakt on 27.11.2021.
//

#pragma once

#include "GameObjectCommand.h"
#include "StateCommand.h"

struct PurchaseCommand : virtual GameObjectCommand, virtual StateCommand {
    PurchaseCommand(const GameObject &object, WorldState &state) : GameObjectCommand(object), StateCommand(state) {}
    explicit PurchaseCommand(WorldState &state) : StateCommand(state) {}
};