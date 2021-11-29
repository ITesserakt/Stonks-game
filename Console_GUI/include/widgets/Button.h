#pragma once

#include "HoverableWidget.h"

struct Button : virtual HoverableWidget {
    Button(std::string name, int index);

    template <typename C>
    Button(std::string name, int index, C &&command) : Button(name, index) {
        applyAction(std::forward<C>(command));
    }

    void show() override;

    UISize getSize() override;
};
