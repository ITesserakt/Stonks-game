#pragma once

#include "widgets/Button.h"
#include "widgets/Label.h"
#include "GameWidgets/Purchase.h"
#include "GameWidgets/Sale.h"
#include "Player.h"
#include "Canvas.h"
#include "World.h"

#define FAST_TRAVEL_MENUS 3
using canvases = std::vector<std::shared_ptr<Canvas>>;
enum SceneNames {
    MainMenu = 0, GameField, Inventory, Guide, Settings
};

void setupCurses();

void checkWindowSize();

void createCanvas(const std::string &name, const Align &al, canvases &scenes,
                  WorldState &state,
                  std::function<void(WorldState &, canvases &)> setupCanvas);

void setupMainMenu(WorldState &state, canvases &scenes);

void setupGameField(WorldState& state, canvases &scenes);

void setupInventory(WorldState &state, canvases &scenes);

void setupGuide(WorldState &state, canvases &scenes);

void setupSettings(WorldState& s, canvases &scenes);