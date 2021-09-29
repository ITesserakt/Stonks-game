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
    std::shared_ptr<Canvas> currCan;

public:
    Button(std::string name, int index, std::function<void(
            HoverableWidget&)> f) :
        Widget(std::move(name)),
        ColorWidget(name), HoverableWidget(index, f) {
        isBlowing = false;
        size = {getWidth(name), getHeight(name)};
    }

    void show() override;

    UISize getSize() override { return size; };
};