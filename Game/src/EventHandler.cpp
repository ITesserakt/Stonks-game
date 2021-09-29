//
// Created by tesserakt on 29.09.2021.
//

#include "EventHandler.h"
#include "utils.h"

void EventHandler::startLoop() {
    Event event;
    while (producer.waitEvent(event)) {
        switch (event.type) {
            case Event::KEY_ENTERED:
                switch (event.keyEntered.key) {
                    case KEY_UP:
                        (*currentScene)->changeActiveWidget(Direction::UP);
                        break;
                    case KEY_DOWN:
                        (*currentScene)->changeActiveWidget(Direction::DOWN);
                        break;
                    case '\n':
                        (*currentScene)->getActiveWidget()->click();
                        break;
                }
                break;
            case Event::SCENE_CHANGED:
                *currentScene = scenes[event.sceneChanged.nextScene].get();
                break;
            case Event::NO_EVENT:
                break;
        }
        event = Event();
    }
}

EventHandler::EventHandler(const std::vector<std::shared_ptr<Canvas>> &scenes,
                           Canvas *&currentScene) : scenes(scenes),
                                                   currentScene(&currentScene) {}

