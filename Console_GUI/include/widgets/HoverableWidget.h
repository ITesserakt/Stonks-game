#pragma once

#include "PositionedWidget.h"
#include "../EventSequence.h"

class HoverableWidget : public virtual PositionedWidget
{
public:
    // We are on button
    virtual EventSequence onHoverStart() = 0;   // TODO Rename EventSequence to Event
    // We go out of button
    virtual EventSequence onHoverEnd() = 0;
    // We pushed enter on button and started click
    virtual EventSequence click() = 0;          // EventSequence == vector<GuiEvent>
};

// EventSequence => Frontend
// GuiEvent => change color <== widget, color, action == widget.powerOn(Color)