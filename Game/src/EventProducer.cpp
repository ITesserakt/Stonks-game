#include "EventProducer.h"
#include "CreatingGui.h"

bool EventProducer::waitEvent(Event &event) {
    int key = getch();
    if (key == ERR)
        return false;

    if (key >= '1' && key <= FAST_TRAVEL_MENUS + '0') {
        event.type = Event::SCENE_CHANGED;
        event.sceneChanged.nextScene = key - '0' - 1;
        lastScene = key - '0' - 1;
    } else {
        event.type = Event::KEY_ENTERED;
        event.keyEntered.key = key;
    }
    return true;
}
