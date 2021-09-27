#pragma once

#include "HoverableWidget.h"

class Purchase : HoverableWidget {
public:
    Purchase(int index);

    void click() override;

    UISize getSize() override;

    void show() override;
};