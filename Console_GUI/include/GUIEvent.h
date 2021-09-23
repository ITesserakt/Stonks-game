#pragma once

#include "widgets/Widget.h"

class GUIEvent {
    // TODO: this abstract class contains all data to process event
    // Widget &sender;  TODO: this string is not working

public:
    virtual void action() = 0;
};

//class ChangeColor : public GUIEvent {
//    int color;
//
//public:
//    void action() override {
//        widget.powerOn(color);
//    }
//};

// EventSequence onHoverStart() {
//      return EventSequence.empty().andThen(ChangeColor(aboba));
// }

// GuiEvent => change color <== widget, color, action == widget.powerOn(Color)