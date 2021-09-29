#pragma once

#include "widgets/Button.h"
#include "widgets/Label.h"
#include "Purchase.h"
#include "Sale.h"
#include "Player.h"
#include "Canvas.h"
#include "World.h"

#define AMOUNT_OF_MENUS 3
enum SceneNames {
    MainMenu = 0, GameField, Inventory
};

std::vector<std::shared_ptr<Canvas>> createCanvases(std::shared_ptr<Player> guy,
                                                    World &world,
                                                    Canvas *current);

void setupCurses();

void checkWindowSize();