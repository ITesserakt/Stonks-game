#pragma once

#include "widgets/Button.h"
#include "widgets/Purchase.h"
#include "widgets/PlainText.h"
#include "Player.h"
#include "Canvas.h"

#define AMOUNT_OF_MENUS 3

std::vector<std::shared_ptr<Canvas>> createCanvases(std::shared_ptr<Player> guy);

void setupCurses();

void checkWindowSize();