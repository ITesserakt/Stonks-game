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
#include <iomanip>

int main(int argc, char **argv) {

    bool debugFlag = false;
    if (argc >= 2)
        debugFlag = std::string("-d") == std::string(argv[1]);
    setupCurses();
    checkWindowSize();

    auto mainMenu = std::make_shared<Canvas>("MainMenu", Centered);
    auto gameField = std::make_shared<Canvas>("GameField", Left);
    auto inventory = std::make_shared<Canvas>("Inventory", Left);
    auto guide = std::make_shared<Canvas>("Guide", Left);
    std::vector<std::shared_ptr<Canvas>> scenes = {mainMenu, gameField, inventory, guide};
    WorldState state(*scenes[SceneNames::MainMenu], 3);

    setupMainMenu(state, *mainMenu, *gameField, *guide);
    setupGameField(state, *gameField);
    setupInventory(state, *inventory);
    setupGuide(state, *guide, *mainMenu);

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
                    auto &item = state.getWorld().viewItem(slot);
                    std::stringstream ss;
                    if (debugFlag) { ss << item; }
                    else { ss << item.fullName() << (item.timesSold > 0 ? "*" : ""); }
                    purch->setItemId(slot);
                    purch->setName(ss.str());
                    purch->setCost(item.cost);
                }
                std::ostringstream os;
                os << "Balance: $" << std::setprecision(4) << state.getPlayer().getBalance();
                scenes[SceneNames::GameField]->getChildWithName("Money Amount")->as<Label>()->changeText(os.str());
                if (0/*state.isWin()*/)
                    scenes[SceneNames::GameField]->getChildWithName("Win Message")->as<HoverableWidget>()->hide(false);
            } else if (state.getCurrentScene() == *scenes[SceneNames::Inventory].get()) {
                auto sales = scenes[SceneNames::Inventory]->getChildrenRecursively<Sale>();
                auto items = state.getPlayer().getSlots();
                for (const auto &sale: sales) {
                    sale->setName("");
                    sale->setItemId(-1);
                }
                for (auto[itemId, sale]: ranges::views::zip(items, sales)) {
                    sale->setItemId(itemId);
                    std::stringstream ss;
                    auto item = state.getPlayer().viewItem(itemId);
                    ss << item.fullName() << ", cost: $" << item.cost;
                    sale->setName(ss.str());
                }
            }
            state.getCurrentScene().show();
            refresh();
        }
    }).detach();

    handler.startLoop();
}