#pragma once

#include "PositionedWidget.h"

class Group : public virtual PositionedWidget {
public:
    UISize getSize() override;

    void show() override;

    explicit Group(std::string name);
};