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

void setupMainMenu(WorldState &state, canvases &scenes);

void setupGameField(WorldState& state, Canvas& gameField);

void setupInventory(WorldState& state, Canvas& inventory);

void setupGuide(WorldState &state, Canvas &guide, Canvas& mainMenu);

void setupSettings(WorldState& state, canvases &scenes);