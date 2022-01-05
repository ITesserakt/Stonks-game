#pragma once

#include "EventProducer.h"
#include "WorldState.h"

class EventHandler {
private:
    EventProducer producer;
    WorldState& state;

public:
    explicit EventHandler(WorldState &state);

    void startLoop();
};