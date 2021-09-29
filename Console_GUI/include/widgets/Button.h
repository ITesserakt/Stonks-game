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
    std::function<void(Button&)> todo;
    std::shared_ptr<Canvas> currCan;

public:
    Button(std::string name, int index, std::function<void(Button&)> f) :
        Widget(std::move(name)),
        ColorWidget(name),
        HoverableWidget(index),
        todo(std::move(f)) {
        isBlowing = false;
        size = {getWidth(name), getHeight(name)};
    }

    void show() override;

    UISize getSize() override { return size; };

    void click() override;
};