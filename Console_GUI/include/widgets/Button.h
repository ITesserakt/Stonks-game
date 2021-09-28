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
    ButtonSpec destiny;
    int nextCanvasID;

public:
    Button(std::string name, ButtonSpec forWhat, int index) : Widget(
            std::move(name)), HoverableWidget(index), destiny(forWhat),
            nextCanvasID(ID_NOT_SET) {
        isBlowing = false;
        size = {getWidth(name), getHeight(name)};
    }

    void setNextCanID(int ID) {
        if (destiny != CanvasChanger)
            throw std::runtime_error("Wrong destiny of Button\n");
        nextCanvasID = ID;
    }

    void show() override;

    UISize getSize() override { return size; };

    void click(Event &event) override;
};