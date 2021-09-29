#include "thread"
#include "CreatingGui.h"
#include "Canvas.h"
#include "World.h"
#include "Player.h"
#include "AI.h"
#include "EventHandler.h"
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

    bool gameRunning = true;
    Canvas* current;
    auto scenes = createCanvases(I, Earth, current);
    current = scenes[SceneNames::MainMenu].get();

    auto handler = EventHandler(scenes, current);

    std::thread guiThread([&]() {
        while (gameRunning) {
            usleep(16666);
            clear();
            if (current == scenes[SceneNames::GameField].get()) {
                auto slots = Earth.getSlots();
                auto purches = scenes[SceneNames::GameField]->getChildrenRecursively<Purchase>();
                for (auto[slot, purch]: ranges::views::zip(slots, purches)) {
                    purch->setItemId(slot);
                    purch->setName(Earth.viewItem(slot).fullName());
                }
            }
            current->show();
            refresh();
        }
    });

    handler.startLoop();
    curs_set(1);
    endwin();
    guiThread.join();
    //worldThread.join();
}