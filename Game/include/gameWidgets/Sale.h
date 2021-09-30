#pragma once

#include "ButtonWithObj.h"
#include "Canvas.h"

class Sale : public ButtonWithObj {
public:
    Sale(int index, WorldState& state, std::function<void(WorldState &,
                                                          Sale &)> f);

    UISize getSize() override;
    void show() override;
};