#pragma once

#include "HoverableWidget.h"
#include "utils.h"
#include <ncurses.h>
#include <string>

class Button : public HoverableWidget
{
public:
    Button(std::string name);
    void show() override;
    Size<unsigned int> getSize() override { return size; };
    // light a button
    virtual void turnOn(int color = COLOR_GREEN) override { col = color; isBlowing = true; };
    // put out the light
    virtual void turnOff() override { isBlowing = false; };

    // Unimplemented methods TODO
    EventSequence onHoverEnd() override;
    EventSequence onHoverStart() override;
    EventSequence click() override;
};