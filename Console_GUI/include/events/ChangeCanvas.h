#pragma once

#include "GUIEvent.h"
#include "widgets/Button.h"

class ChangeCanvas : public virtual GUIEvent {
public:
    ChangeCanvas(std::shared_ptr<Button> father) : GUIEvent(father) {}

    void action() override {
        sender->getCanvas();
    }
};