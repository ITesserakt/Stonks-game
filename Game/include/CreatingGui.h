#pragma once

#include "widgets/Button.h"
#include "widgets/Label.h"
#include "Player.h"
#include "Canvas.h"
#include "World.h"
#include "WorldState.h"

using Canvases = std::vector<std::shared_ptr<Canvas>>;

enum SceneNames {
    MainMenu = 0, GameField, Inventory, Guide, Settings
};

void setupMainMenu(WorldState &state, Canvases &scenes);

void setupGameField(WorldState& state, Canvases &scenes);

void setupInventory(WorldState &state, Canvases &scenes);

void setupGuide(WorldState &state, Canvases &scenes);

void setupSettings(WorldState& s, Canvases &scenes);