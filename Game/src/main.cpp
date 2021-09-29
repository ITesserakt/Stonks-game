#include "thread"
#include "CreatingGui.h"
#include "Canvas.h"
#include "Player.h"
#include "EventHandler.h"
#include "WorldState.h"
#include <ncurses.h>
#include <unistd.h>
#include <random>
#include <iostream>
#include <sstream>

int main() {
    setupCurses();
    checkWindowSize();

    auto mainMenu = std::make_shared<Canvas>("MainMenu", Centered);
    auto gameField = std::make_shared<Canvas>("GameField", Left);
    auto inventory = std::make_shared<Canvas>("Inventory", Left);
    std::vector<std::shared_ptr<Canvas>> scenes = {mainMenu, gameField,
                                                   inventory};
    WorldState state(*scenes[SceneNames::MainMenu], 1);

    setupMainMenu(state, *mainMenu, *gameField);
    setupGameField(state, *gameField);
    setupInventory(state, *inventory);

    auto handler = EventHandler(scenes, state);

    std::thread([&]() {
        while (state.running()) {
            usleep(16666);
            clear();
            if (state.getCurrentScene() == *scenes[SceneNames::GameField]) {
                auto slots = state.getWorld().getSlots();
                auto purches = scenes[SceneNames::GameField]->getChildrenRecursively<Purchase>();
                for (const auto &purch: purches) {
                    purch->setName("");
                    purch->setItemId(-1);
                    purch->setCost(0);
                }
                for (auto[slot, purch]: ranges::views::zip(slots, purches)) {
                    purch->setItemId(slot);
                    purch->setName(state.getWorld().viewItem(slot).fullName());
                    purch->setCost(state.getWorld().viewItem(slot).cost);
                }
            }
            else if (state.getCurrentScene() == *scenes[SceneNames::Inventory].get()) {
                auto sales = scenes[SceneNames::Inventory]->getChildrenRecursively<Sale>();
                auto items = state.getPlayer().getSlots();
            }
            state.getCurrentScene().show();
            refresh();
        }
    }).detach();

    std::thread([&]() {
        std::random_device seed;
        std::mt19937 randie(seed());
        while (state.running()) {
            usleep(1000000 + randie() % 9000000);
            state.getWorld().fillUp();
        }
    }).detach();

    std::thread([&]() {
        std::random_device seed;
        std::mt19937 randie(seed());
        while (state.running()) {
            usleep(1000000 + randie() % 9000000);
            auto &randomBot = state.getRandomBot();
            if (state.getWorld().getSlots().size() != 0)
                randomBot.buyItem(
                        state.getWorld().takeItem(randomBot.predict()));
        }
    }).detach();

    handler.startLoop();
}