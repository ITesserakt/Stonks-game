#pragma once

#include <utility>

#include "HoverableWidget.h"

class Purchase : public HoverableWidget {
public:
    Purchase(int index);

    void click(Event &event) override;

    UISize getSize() override;

    void setName(std::string newName) { name = std::move(newName); }

    void show() override;
};