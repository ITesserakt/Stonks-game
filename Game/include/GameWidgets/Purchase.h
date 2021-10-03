#pragma once

#include <utility>

#include "GameWidgets/ButtonWithObj.h"
#include "GameObject.h"

class Purchase : public ButtonWithObj{
public:
    Purchase(int index, WorldState& state, std::function<void(WorldState&, Purchase&)> f);

    UISize getSize() override;

    void show() override;
};