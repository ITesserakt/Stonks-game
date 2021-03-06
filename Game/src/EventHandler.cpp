//
// Created by tesserakt on 29.09.2021.
//

#include "EventHandler.h"
#include "game_widgets/Sale.h"
#include "utils.h"

void EventHandler::startLoop() {
    Event event;
    while (state.running() && producer.waitEvent(event)) {
        switch (event.type) {
            case Event::KEY_ENTERED:
                switch (event.keyEntered.key) {
                    case KEY_UP: state.getCurrentScene().changeActiveWidget(Direction::UP); break;
                    case KEY_DOWN: state.getCurrentScene().changeActiveWidget(Direction::DOWN); break;
                    case '\n': state.getCurrentScene().getActiveWidget()->click(); break;
                    case KEY_RIGHT:
                        if (state.getCurrentScene().getActiveWidget()->is<Sale>())
                            state.getCurrentScene().getActiveWidget()->as<Sale>()->growPrice(10);
                        break;
                    case KEY_LEFT:
                        if (state.getCurrentScene().getActiveWidget()->is<Sale>())
                            state.getCurrentScene().getActiveWidget()->as<Sale>()->growPrice(-10);
                        break;
                }
                break;
            case Event::SCENE_CHANGED:
            case Event::NO_EVENT: break;
        }
        event = Event();
    }
}

EventHandler::EventHandler(WorldState &state) : state(state) {}
