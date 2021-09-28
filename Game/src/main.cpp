#include "thread"
#include "EventConductor.h"
#include "CreatingGui.h"
#include "Canvas.h"
#include "World.h"
#include "Player.h"
#include "AI.h"
#include <ncurses.h>
#include <unistd.h>

std::string json = R"({
        "Objects" : [{
            "name" : "car",
            "descriptions": [{
                "value" : "red",
                "multiplier" : 1.05
            }, {
                "value": "shiny",
                        "multiplier": 2
            }, {
                "value" : "dirty",
                        "multiplier": 0.75
            }],
            "cost": 10000
        }, {
            "name": "dress",
            "descriptions": [{
                "value": "gucci",
                "multiplier": 15
            }],
            "cost": 10
        }]
})";

int main() {
    setupCurses();

    checkWindowSize();

    auto Earth = World(ObjectFactory(nlohmann::json::parse(json)));
    auto I = std::make_shared<Player>();
    auto Bot = std::make_shared<AI>(Earth);
    Earth.addGamer(I);
    Earth.addGamer(Bot);
    //Earth.fillUp();           // Buggie function call

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
            for (unsigned int i = 0; i < slots.size(); i++) {
                auto slot = slots[i];
                auto purch = purches[i];
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

    endwin();
    guiThread.join();
    //worldThread.join();
}