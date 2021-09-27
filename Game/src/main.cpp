#include "thread"
#include "Frontend.h"
#include "EventConductor.h"
#include "CreatingGui.h"
#include "Canvas.h"
#include <ncurses.h>
#include <unistd.h>

int main() {
    initscr();
    start_color();
    noecho();                       // Removes characters when typed
    curs_set(0);                    // Removes cursor
    keypad(stdscr, true);

    if (getmaxx(stdscr) < 80 || getmaxy(stdscr) < 24) {
        endwin();
        printf("Your terminal should be bigger or equal to 80x24 size\n");
        return -1;
    }

    std::vector<std::shared_ptr<Canvas>> scenes = createCanvases();

    auto current = scenes[0];
    current->firstOnHover();

    bool gameRunning = true;
    EventConductor director;
    Event game;

    std::thread guiThread([&]() {
        while (gameRunning) {
            usleep(16666);
            erase();
            current->show();
            refresh();
        }
    });

    while (director.waitEvent(game)) {
        switch (game.type) {
            case Event::key_up:
                current->changeActiveWidget(toTheTop);
                break;
            case Event::key_down:
                current->changeActiveWidget(toTheBot);
                break;
            case Event::key_enter:
                current->getActiveWidget()->click();
                break;
            case Event::changeScene:
                current = scenes[game.changingScene.nextScene - 1];
                break;
            case Event::noEvent:
                break;
        }
        game = Event();
    }

    endwin();
    guiThread.join();
}