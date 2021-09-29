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
    auto scenes = createCanvases(I, Earth);
    auto current = scenes[SceneNames::MainMenu].get();
    auto butPl = std::make_shared<Button>("play", 0, [&](auto& x) {
        current = scenes[SceneNames::GameField].get();
    });
    auto butQ = std::make_shared<Button>("quit", 2, [&](auto &x) {
        gameRunning = false;
        endwin();
        exit(0);
    });
    auto butSt = std::make_shared<Button>("settings", 1, [](auto& x) {});
    scenes[SceneNames::MainMenu]->bind(butPl);
    scenes[SceneNames::MainMenu]->bind(butSt);
    scenes[SceneNames::MainMenu]->bind(butQ);
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