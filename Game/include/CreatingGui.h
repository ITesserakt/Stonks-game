#pragma once

#include "widgets/Button.h"
#include "widgets/Label.h"
#include "GameWidgets/Purchase.h"
#include "GameWidgets/Sale.h"
#include "Player.h"
#include "Canvas.h"
#include "World.h"

using Canvases = std::vector<std::shared_ptr<Canvas>>;

enum SceneNames {
    MainMenu = 0, GameField, Inventory, Guide, Settings
};

void createCanvas(const std::string &name, const Align &al, Canvases &scenes,
                  WorldState &state,
                  std::function<void(WorldState &, Canvases &)> setupCanvas);

void setupMainMenu(WorldState &state, Canvases &scenes);

void setupGameField(WorldState& state, Canvases &scenes);

void setupInventory(WorldState &state, Canvases &scenes);

void setupGuide(WorldState &state, Canvases &scenes);

void setupSettings(WorldState& s, Canvases &scenes);