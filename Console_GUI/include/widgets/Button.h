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
    int width;

public:
    Button(std::string name);
    void show();
    Rect<unsigned int> getSize();
    // light a button
    void turnOn(int color = COLOR_GREEN) { col = color; isBlowing = true; };
    // put out the light
    void turnOff() { isBlowing = false; };

    // Unimplemented methods TODO
    EventSequence onHoverEnd();
    EventSequence onHoverStart();
    EventSequence click();
};