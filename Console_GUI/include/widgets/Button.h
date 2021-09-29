#pragma once

#include "HoverableWidget.h"
#include "utils.h"
#include <ncurses.h>
#include <string>
#include <utility>
#include "ColorWidget.h"

class Button : public virtual HoverableWidget {
private:
    std::function<void(WorldState&, Button&)> todo;
    std::shared_ptr<Canvas> currCan;

public:
    Button(std::string name, int index, WorldState& state, std::function<void(WorldState&, Button&)> f) :
        Widget(std::move(name)),
        ColorWidget(name),
        HoverableWidget(index, state),
        todo(std::move(f)) {
        isBlowing = false;
        size = {getWidth(name), getHeight(name)};
    }

    void show() override;

    UISize getSize() override { return size; };

    void click() override;
};