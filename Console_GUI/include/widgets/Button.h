#pragma once

#include "HoverableWidget.h"
#include "Utils.h"
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
    void turnOn(int color = Green) { col = color; isBlowing = true; };
    // put out the light
    void turnOff() { col = NoColor; isBlowing = false; };

    // Unimplemented methods TODO
    EventSequence onHoverEnd();
    EventSequence onHoverStart();
    EventSequence click();
};