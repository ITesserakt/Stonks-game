#pragma once

#include "widgets/Widget.h"

class GUIEvent {
    // TODO: this abstract class contains all data to process event
    // Widget &sender;  TODO: this string is not working

public:
    virtual void action() = 0;
};