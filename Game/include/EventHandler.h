#pragma once

#include "EventProducer.h"
#include "Canvas.h"

class EventHandler {
private:
    std::vector<std::shared_ptr<Canvas>> scenes;
    Canvas** currentScene;
    EventProducer producer;
public:
    EventHandler(const std::vector<std::shared_ptr<Canvas>> &scenes,
                 Canvas *&currentScene);

    void startLoop();
};