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

    auto Earth = World(ObjectFactory::fromFile("../share/objects.json"));
    auto I = std::make_shared<Player>();
    auto Bot = std::make_shared<AI>(Earth);
    Earth.addGamer(I);
    Earth.addGamer(Bot);
    Earth.fillUp();           // Buggie function call

    auto scenes = createCanvases(I, Earth);

    auto current = scenes[0];
    current->firstOnHover();

    bool gameRunning = true;
    EventConductor director;
    Event game;

    std::thread guiThread([&]() {
        while (gameRunning) {
            usleep(16666);
            erase();
            auto slots = Earth.getSlots();
            auto purches = scenes[SceneNames::GameField]->getChildrenRecursively<Purchase>();
            for (auto [slot, purch]: ranges::views::zip(slots, purches)) {
                purch->setName(Earth.viewItem(slot).fullName());
            }
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
                current->getActiveWidget()->click(game);
                /*break; doesn't need here*/
            case Event::changeScene:
                current = scenes[game.changingScene.nextScene - 1];
                break;
            case Event::noEvent:
                break;
        }
        game = Event();
    }

    curs_set(1);
    endwin();
    guiThread.join();
    //worldThread.join();
}