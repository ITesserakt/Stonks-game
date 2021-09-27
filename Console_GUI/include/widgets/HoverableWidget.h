#pragma once

#include "PositionedWidget.h"
#include "ColorWidget.h"

class HoverableWidget : public virtual PositionedWidget, public virtual ColorWidget {
protected:
    int tabIndex;
public:
    // We are on button
    void onHoverStart(){
        turnOn(COLOR_GREEN);
    }

    // We go out of button
    void onHoverEnd() {
        turnOff();
    }

    // We pushed enter on button and started click
    virtual void click() = 0;

    int getTabIndex() const { return tabIndex; }

    explicit HoverableWidget(int index): tabIndex(index) {}
};

// EventSequence => Frontend
// GuiEvent => change color <== widget, color, action == widget.powerOn(Color)