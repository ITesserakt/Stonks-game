//
// Created by tesserakt on 29.09.2021.
//

#include "EventHandler.h"
#include "utils.h"

void EventHandler::startLoop() {
    Event event;
    while (state.running() && producer.waitEvent(event)) {
        switch (event.type) {
            case Event::KEY_ENTERED:
                switch (event.keyEntered.key) {
                    case KEY_UP:
                        state.getCurrentScene().changeActiveWidget(
                                Direction::UP);
                        break;
                    case KEY_DOWN:
                        state.getCurrentScene().changeActiveWidget(
                                Direction::DOWN);
                        break;
                    case '\n':
                        state.getCurrentScene().getActiveWidget()->click();
                        break;
                    case KEY_RIGHT:
                        //FIXME
                        break;
                    case KEY_LEFT:
                        //FIXME
                        break;
                }
                break;
            case Event::SCENE_CHANGED:
                state.changeCurrentScene(*scenes[event.sceneChanged.nextScene]);
                break;
            case Event::NO_EVENT:
                break;
        }
        event = Event();
    }
}

EventHandler::EventHandler(const std::vector<std::shared_ptr<Canvas>> &scenes,
                           WorldState &state) : scenes(scenes), state(state) {}

