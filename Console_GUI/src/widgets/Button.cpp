#include "widgets/Button.h"
#include "utils.h"
#include "Event.h"
#include <cstdlib>
#include <iostream>

void Button::show() {
    if (isHidden) return;

    move(position.y, position.x);
    if (isBlowing) { init_pair(widgetId, COLOR_BLACK, col); }
    if (size.height == 1) {
        if (isBlowing) { attron(COLOR_PAIR(widgetId)); }
        printw("[ %s ]", name.c_str());
        if (isBlowing) { attroff(COLOR_PAIR(widgetId)); }
    }
}

void Button::click(Event &event) {
    if (destiny == Quiter) {
        endwin();
        std::cout << "See you later" << std::endl;
        exit(0);
    } else if (destiny == CanvasChanger && nextCanvasID != ID_NOT_SET) {
        event.type = Event::changeScene;
        // Shift on 1 is needed because user input 1 and we should go to the
        // canvas with ID 0
        event.changingScene.nextScene = nextCanvasID + 1;
    }
}