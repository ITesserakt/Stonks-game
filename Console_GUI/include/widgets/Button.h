#pragma once

#include "HoverableWidget.h"
#include "utils.h"
#include <ncurses.h>
#include <string>
#include <utility>
#include "ColorWidget.h"

enum ButtonSpec {
    CanvasChanger, Quiter
};

class Button : public virtual HoverableWidget {
private:
    ButtonSpec destiny;

    UISize size;

public:
    Button(std::string name, ButtonSpec forWhat, int index) : Widget(
            std::move(name)), HoverableWidget(index), destiny(forWhat) {
        isBlowing = false;
        size = {getWidth(name), getHeight(name)};
    }

    void show() override;

    UISize getSize() override { return size; };

    void click() override;
};