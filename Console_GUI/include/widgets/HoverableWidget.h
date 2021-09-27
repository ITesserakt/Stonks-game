#pragma once

#include "PositionedWidget.h"
#include "../EventSequence.h"

class HoverableWidget : public virtual PositionedWidget {
protected:
    int tabIndex;
public:
    // We are on button
    virtual std::unique_ptr<GUIEvent> onHoverStart() = 0;

    // We go out of button
    virtual std::unique_ptr<GUIEvent> onHoverEnd() = 0;

    // We pushed enter on button and started click
    virtual std::unique_ptr<GUIEvent> click() = 0;

    int getTabIndex() const { return tabIndex; }

    HoverableWidget(int index): tabIndex(index) {}
};

// EventSequence => Frontend
// GuiEvent => change color <== widget, color, action == widget.powerOn(Color)