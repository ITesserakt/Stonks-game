#pragma once

#include "HoverableWidget.h"
#include "utils.h"
#include <ncurses.h>
#include <string>
#include <utility>

enum ButtonSpec {
    CanvasChanger, Quiter
};

class Button : public HoverableWidget {
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

    // light a button
    virtual void turnOn(int color = COLOR_GREEN) override {
        col = color;
        isBlowing = true;
    };

    // put out the light
    virtual void turnOff() override { isBlowing = false; };

    // TODO Unimplemented methods
    std::unique_ptr<GUIEvent> onHoverEnd() override;

    std::unique_ptr<GUIEvent> onHoverStart() override;

    std::unique_ptr<GUIEvent> click() override;
};