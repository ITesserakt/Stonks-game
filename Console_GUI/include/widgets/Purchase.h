#pragma once

#include "HoverableWidget.h"

class Purchase : HoverableWidget {
public:
    Purchase();

    void click() override;

    UISize getSize() override;

    void show() override;
};