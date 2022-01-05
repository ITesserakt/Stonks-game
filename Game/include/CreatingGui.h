#pragma once

#include "Canvas.h"
#include "widgets/dsl/DSL.h"
#include "WorldState.h"

enum SceneNames { MainMenu = 0, GameField, Inventory, Guide, Settings, Statistics };

class Scenes {
private:
    std::vector<std::shared_ptr<Canvas>> scenes;

public:
    Scenes(std::initializer_list<Canvas> list);

    widget::canvas operator[](SceneNames name);
};

void setupMainMenu(WorldState &state, Scenes &scenes);

void setupGameField(WorldState &state, Scenes &scenes);

void setupInventory(WorldState &state, Scenes &scenes);

void setupGuide(WorldState &state, Scenes &scenes);

void setupSettings(WorldState &s, Scenes &scenes);

void setupStatistics(WorldState &state, Scenes &scenes);
