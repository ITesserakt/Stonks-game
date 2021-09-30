#pragma once

#include "EventProducer.h"
#include "Canvas.h"
#include "WorldState.h"

class EventHandler {
private:
    const std::vector<std::shared_ptr<Canvas>>& scenes;
    EventProducer producer;
    WorldState& state;

public:
    EventHandler(const std::vector<std::shared_ptr<Canvas>> &scenes,
                 WorldState& state);

    void startLoop();
};