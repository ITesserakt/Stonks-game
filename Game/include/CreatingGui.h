#pragma once

#include "widgets/Button.h"
#include "widgets/Label.h"
#include "gameWidgets/Purchase.h"
#include "gameWidgets/Sale.h"
#include "Player.h"
#include "Canvas.h"
#include "World.h"

#define FAST_TRAVEL_MENUS 3
enum SceneNames {
    MainMenu = 0, GameField, Inventory, Guide
};

void setupCurses();

void checkWindowSize();

void setupMainMenu(WorldState &state, Canvas &mainMenu, Canvas &gameField,
                   Canvas &guide);

void setupGameField(WorldState& state, Canvas& gameField);

void setupInventory(WorldState& state, Canvas& inventory);

void setupGuide(WorldState &state, Canvas &guide, Canvas& mainMenu);
