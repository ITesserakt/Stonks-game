#pragma once

#include "HoverableWidget.h"
#include "utils.h"
#include <ncurses.h>
#include <string>
#include <utility>
#include "ColorWidget.h"

class Button : public virtual HoverableWidget {
private:
public:
    Button(std::string name, int index, WorldState &state,
           std::function<void(WorldState &, Button &)> f) :
            Widget(std::move(name)),
            ColorWidget(name),
            HoverableWidget(index, state,
                            [f](WorldState &s, HoverableWidget &w) {
                                f(s, *w.as<Button>());
                            }) {
        isBlowing = false;
        size = {getWidth(name), getHeight(name)};
    }

    void show() override;

    UISize getSize() override { return size; };
};