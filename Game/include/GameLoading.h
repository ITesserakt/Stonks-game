#pragma once

#include "Statistics.h"
#include "WorldState.h"

constexpr auto savePath = "../share/save.json";

// This function uses magic feature with constructing object
WorldState loadWorldState();

void loadStatistic();

// Saves World and statistic to json
void saveGameData(WorldState& state);