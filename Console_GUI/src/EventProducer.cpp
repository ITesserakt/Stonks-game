#include "EventProducer.h"
#include <ncurses.h>

constexpr auto fastTravelMenus = 3;

bool EventProducer::waitEvent(Event &event) {
    int key = getch();
    if (key == ERR)
        return false;

    if (key >= '1' && key <= fastTravelMenus + '0') {
        event.type = Event::SCENE_CHANGED;
        event.sceneChanged.nextScene = key - '0' - 1;
        lastScene = key - '0' - 1;
    } else {
        event.type = Event::KEY_ENTERED;
        event.keyEntered.key = key;
    }
    return true;
}
