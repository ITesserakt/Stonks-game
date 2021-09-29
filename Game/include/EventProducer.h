#pragma once

#include "Event.h"

class EventProducer {
    int lastScene = 0;
public:
    bool waitEvent(Event& event);
};