#pragma once

#include "HoverableWidget.h"
#include "utils.h"
#include <ncurses.h>
#include <string>

enum ButtonSpec { CanvasChanger, Quiter};

class Button : public HoverableWidget
{
private:
    ButtonSpec destiny;
    Button(std::string name);

public:
    Button(std::string name, ButtonSpec forWhat) : Button(name)
    { destiny = forWhat; }

    void show() override;
    UISize getSize() override { return size; };
    // light a button
    virtual void turnOn(int color = COLOR_GREEN) override { col = color; isBlowing = true; };
    // put out the light
    virtual void turnOff() override { isBlowing = false; };

    // TODO Unimplemented methods
    std::unique_ptr<GUIEvent> onHoverEnd() override;
    std::unique_ptr<GUIEvent> onHoverStart() override;
    std::unique_ptr<GUIEvent> click() override;
};