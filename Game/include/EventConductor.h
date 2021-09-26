#pragma once

#include "Event.h"

class EventConductor {
    int lastScene = -1;
public:
    bool waitEvent(Event& event);
};