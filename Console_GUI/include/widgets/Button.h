#pragma once

#include "HoverableWidget.h"
#include "utils.h"
#include <ncurses.h>
#include <string>
#include <utility>
#include "ColorWidget.h"

#define ID_NOT_SET -10

enum ButtonSpec {
    CanvasChanger, Quiter
};

class Button : public virtual HoverableWidget {
private:
    std::function<void(Button&)> todo;
    int nextCanvasID;

public:
    Button(std::string name, int index, std::function<void(Button&)> f) :
        Widget(std::move(name)),
        ColorWidget(name),
        HoverableWidget(index),
        todo(std::move(f)),
        nextCanvasID(ID_NOT_SET) {
        isBlowing = false;
        size = {getWidth(name), getHeight(name)};
    }

    void setNextCanID(int ID) {
        nextCanvasID = ID;
    }

    void show() override;

    UISize getSize() override { return size; };

    void click() override;
};