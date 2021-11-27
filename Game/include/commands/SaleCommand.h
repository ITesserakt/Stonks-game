//
// Created by tesserakt on 27.11.2021.
//

#pragma once

#include "GameObjectCommand.h"
#include "StateCommand.h"

struct SaleCommand : virtual GameObjectCommand, virtual StateCommand {
};