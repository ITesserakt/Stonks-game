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

int main(int argc, char** argv) {

    bool debugFlag = false;
    if (argc >= 2)
        debugFlag = std::string("-d") == std::string(argv[1]);
    setupCurses();
    checkWindowSize();

    auto mainMenu = std::make_shared<Canvas>("MainMenu", Centered);
    auto gameField = std::make_shared<Canvas>("GameField", Left);
    auto inventory = std::make_shared<Canvas>("Inventory", Left);
    std::vector<std::shared_ptr<Canvas>> scenes = {mainMenu, gameField, inventory};
    WorldState state(*scenes[SceneNames::MainMenu], 3);

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
                    auto& item = state.getWorld().viewItem(slot);
                    std::stringstream ss;
                    if (debugFlag) { ss << item; }
                    else { ss << item.fullName(); }
                    ss << ", profitness: " << state.getWorld().getProfitness(slot);
                    purch->setItemId(slot);
                    purch->setName(ss.str());
                    purch->setCost(item.cost);
                }
                std::ostringstream os;
                os << "Balance: $" << std::setprecision(4) << state.getPlayer().getBalance();
                scenes[SceneNames::GameField]->getChildWithName("Money Amount")->as<Label>()->changeText(os.str());
            } else if (state.getCurrentScene() == *scenes[SceneNames::Inventory].get()) {
                auto sales = scenes[SceneNames::Inventory]->getChildrenRecursively<Sale>();
                auto items = state.getPlayer().getSlots();
                for (const auto &sale: sales) {
                    sale->setName("");
                    sale->setItemId(-1);
                }
                for (auto[item, sale]: ranges::views::zip(items, sales)) {
                    sale->setItemId(item);
                    sale->setName(state.getPlayer().viewItem(item).fullName());
                }
            }
            state.getCurrentScene().show();
            refresh();
        }
    }).detach();

    std::thread([&]() {
        std::random_device seed;
        std::mt19937 randie(seed());
        useconds_t sleepTime;
        if (debugFlag) {sleepTime = 10000 + randie() % 90000;}
        else {sleepTime = 1000000 + randie() % 9900000;}
        while (state.running()) {
            usleep(sleepTime);
            auto &randomBot = state.getRandomBot();
            auto tradeType = random() % 2;
            if (state.getWorld().getSlots().size() != 0 && randomBot.couldBuy() && tradeType)
                randomBot.buyItem(state.getWorld().takeItem(randomBot.predictToBuy()));
            if (randomBot.getSlots().size() != 0 && state.getWorld().couldPutInto() && !tradeType)
                state.getWorld().putItem(randomBot.takeItem(randomBot.predictToSell()));
        }
    }).detach();

    handler.startLoop();
}