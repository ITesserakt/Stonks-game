#include "EventProducer.h"
#include "CreatingGui.h"

bool EventProducer::waitEvent(Event &event) {
    int key = getch();
    if (key == ERR)
        return false;

    if (key >= '1' && key <= AMOUNT_OF_MENUS + '0') {
        if (lastScene == key - '0' - 1) {
            event.type = Event::NO_EVENT;
            return true;
        }
        event.type = Event::SCENE_CHANGED;
        event.sceneChanged.nextScene = key - '0' - 1;
        lastScene = key - '0' - 1;
    } else {
        event.type = Event::KEY_ENTERED;
        event.keyEntered.key = key;
    }
    return true;
}
