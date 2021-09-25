#pragma once

#include "HoverableWidget.h"
#include "utils.h"
#include <ncurses.h>
#include <string>

class Button : public HoverableWidget
{
protected:
    bool isBlowing;
    int col;

public:
    Button(std::string name);
    void show() override;
    Size<unsigned int> getSize() override { return size; };
    // light a button
    void turnOn(int color = COLOR_GREEN) { col = color; isBlowing = true; };
    // put out the light
    void turnOff() { isBlowing = false; };

    // Unimplemented methods TODO
    EventSequence onHoverEnd() override;
    EventSequence onHoverStart() override;
    EventSequence click() override;
};