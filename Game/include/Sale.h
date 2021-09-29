#pragma once

#include "widgets/HoverableWidget.h"
#include "Canvas.h"

class Sale : public HoverableWidget {
public:
    Sale(int index, WorldState& s, std::function<void(WorldState&, HoverableWidget&)> f);

    UISize getSize() override;
    void show() override;
};