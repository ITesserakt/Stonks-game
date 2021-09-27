#include "thread"
#include "EventConductor.h"
#include "CreatingGui.h"
#include "Canvas.h"
#include "World.h"
#include "Player.h"
#include "AI.h"
#include <ncurses.h>
#include <unistd.h>


int main() {
    setupCurses();

    checkWindowSize();

    auto Earth = World();
    auto I     = std::make_shared<Player>();
    auto Bot   = std::make_shared<AI>(Earth);
    Earth.addGamer(I);
    Earth.addGamer(Bot);

    auto scenes = createCanvases(I);

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