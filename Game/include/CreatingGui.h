#pragma once

#include "widgets/Button.h"
#include "widgets/Purchase.h"
#include "widgets/Label.h"
#include "Player.h"
#include "Canvas.h"
#include "World.h"

#define AMOUNT_OF_MENUS 3

std::vector<std::shared_ptr<Canvas>> createCanvases(std::shared_ptr<Player> guy,
                                                    const World & world);

void setupCurses();

void checkWindowSize();