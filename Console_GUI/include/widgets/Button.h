#pragma once

#include <ncurses.h>
#include <string>
#include <utility>

#include "ColorWidget.h"
#include "HoverableWidget.h"
#include "utils.h"

class Button : public virtual HoverableWidget {
private:
public:
    Button(std::string name, int index);

    template <typename C>
    Button(std::string name, int index, C &&command) : Button(name, index) {
        applyAction(std::forward<C>(command));
    }

    void show() override;

    UISize getSize() override;
};
