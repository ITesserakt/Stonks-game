#pragma once

#include "widgets/HoverableWidget.h"
#include "Canvas.h"

class Sale : public HoverableWidget {
public:
    Sale(int index, std::function<void(HoverableWidget&)> f);

    UISize getSize() override;
    void show() override;
};