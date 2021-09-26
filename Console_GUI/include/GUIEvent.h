#pragma once

#include <utility>

#include "widgets/Widget.h"

class GUIEvent {
protected:
    // TODO: this abstract class contains all data to process event
     std::shared_ptr<Widget> sender;
     GUIEvent(std::shared_ptr<Widget> father): sender(std::move(father)) {}

public:
     virtual void action() = 0;
     virtual ~GUIEvent() = default;
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