#include "EventConductor.h"

bool EventConductor::waitEvent(Event &event) {
    int key = getch();
    if (key == '\n')
        event.type = Event::key_enter;
    else if (key == KEY_UP)
        event.type = Event::key_up;
    else if (key == KEY_DOWN)
        event.type = Event::key_down;
    else if (key >= '1' && key <= '9') {
        if (lastScene != key - '0') {
            event.type = Event::changeScene;
            event.changingScene.nextScene = key - '0';
        }
    }
    else
        event.type = Event::noEvent;
    return true;
}
