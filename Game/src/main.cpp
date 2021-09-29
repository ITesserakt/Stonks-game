#include "thread"
#include "CreatingGui.h"
#include "Canvas.h"
#include "World.h"
#include "Player.h"
#include "AI.h"
#include "EventHandler.h"
#include <ncurses.h>
#include <unistd.h>
#include <random>
#include <iostream>
#include <sstream>

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
    Canvas *current;
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
                for (const auto &purch: purches) {
                    purch->setName("");
                    purch->setItemId(-1);
                    purch->setCost(0);
                }
                for (auto[slot, purch]: ranges::views::zip(slots, purches)) {
                    purch->setItemId(slot);
                    purch->setName(Earth.viewItem(slot).fullName());
                    purch->setCost(Earth.viewItem(slot).cost);
                }
            }
            else if (current == scenes[SceneNames::Inventory].get()) {
                auto sales = scenes[SceneNames::Inventory]->getChildrenRecursively<Sale>();
                auto items = I->getSlots();
            }
            current->show();
            refresh();
        }
    });

    std::thread worldThread([&]() {
        std::random_device seed;
        std::mt19937 randie(seed());
        while (gameRunning) {
            usleep(1000000 + randie() % 9000000);
            Earth.fillUp();
        }
    });

    std::thread aiThread([&]() {
        std::random_device seed;
        std::mt19937 randie(seed());
        while (gameRunning) {
            usleep(1000000 + randie() % 9000000);
            if (Earth.getSlots().size() != 0)
                Bot->buyItem(Earth.takeItem(Bot->predict()));
        }
    });

    handler.startLoop();
    curs_set(1);
    endwin();
    guiThread.join();
    worldThread.join();
    aiThread.join();
}