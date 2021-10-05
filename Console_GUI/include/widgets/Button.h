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
    Button(std::string name, int index, WorldState &state, std::function<void(WorldState &, Button &)> f);

    Button(std::string name, int index, WorldState &state);

    template<typename F>
    void applyAction(F func) {
        todo = [func](WorldState &s, HoverableWidget &w) {
            func(s, *w.as<Button>());
        };
    }

    void show() override;

    UISize getSize() override;
};