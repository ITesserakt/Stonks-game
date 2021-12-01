#pragma once

#include "PositionedWidget.h"
#include "BindableWidget.h"

struct Group : PositionedWidget, BindableWidget {
    UISize getSize() override;

    void show() override;

    explicit Group(const std::string &name);
};