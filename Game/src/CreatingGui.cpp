#include "CreatingGui.h"
#include "gameWidgets/Purchase.h"
#include "widgets/Widget.h"
#include "widgets/MessageBox.h"
#include "WorldState.h"
#include <iostream>
#include <vector>

constexpr auto EXTRA_SLOTS = 2;

void setupCurses() {
    initscr();
    start_color();
    noecho();                       // Removes characters when typed
    curs_set(0);                    // Removes cursor
    keypad(stdscr, true);
}

void checkWindowSize() {
    if (getmaxx(stdscr) < 80 || getmaxy(stdscr) < 24) {
        endwin();
        throw std::runtime_error("Your terminal should be bigger or equal to 80x24 size\n");
    }
}

void setupMainMenu(WorldState &state, Canvas &mainMenu, Canvas &gameField,
                   Canvas &guide) {
    auto label1 = std::make_shared<Label>("game name", "STONKS GAME\n");
    label1->turnOn(COLOR_YELLOW);
    auto butPl = std::make_shared<Button>("play", 0, state, [&](WorldState &state, Button &x) {
        state.changeCurrentScene(gameField);
    });
    auto butGd = std::make_shared<Button>("guide", 1, state, [&](auto &_, auto &x) {
        state.changeCurrentScene(guide);
    });
    auto butQ = std::make_shared<Button>("quit", 2, state, [&](WorldState &s, auto &x) {
        clear();
        endwin();
        std::cout << "See you later ;)" << std::endl;
        exit(0);
    });
    mainMenu.bind(label1);
    mainMenu.bind(butPl);
    mainMenu.bind(butGd);
    mainMenu.bind(butQ);
}

void setupGameField(WorldState &state, Canvas &gameField) {
    auto label2 = std::make_shared<Label>("stocks", "Game field\n");
    label2->turnOn(COLOR_YELLOW);
    gameField.bind(label2);

    auto balance = std::make_shared<Label>("Money Amount", "Balance: \n");
    gameField.bind(balance);

    for (unsigned long i = 0; i < state.getWorld().getSlots().size(); i++) {
        auto purch = std::make_shared<Purchase>(i, state, [&](WorldState &s, Purchase &x) {
            if (x.getItemId() != static_cast<unsigned int>(-1)
                && s.getPlayer().couldBuy()
                && s.getPlayer().getBalance() > s.getWorld().viewItem(x.getItemId()).cost) {
                s.getPlayer().buyItem(s.getWorld().takeItem(x.getItemId()));
                x.setCost(0);
                x.setName("");
            }
        });
        gameField.bind(purch);
    }

    auto winMessage = std::make_shared<MessageBox>("Win Message", "You have win!");
    winMessage->hide(true);
    gameField.bind(winMessage);
}

void setupInventory(WorldState &state, Canvas &inventory) {
    auto label = std::make_shared<Label>("inventory", "Inventory\n");
    inventory.bind(label);
    label->turnOn(COLOR_YELLOW);

    for (unsigned long i = 0; i < state.getPlayer().getInventorySize(); i++) {
        auto sale = std::make_shared<Sale>(i, state, [&](WorldState &s, Sale &x) {
            if (x.getItemId() != static_cast<unsigned int>(-1) && s.getWorld().couldPutInto()) {
                s.getWorld().putItem(s.getPlayer().sellItem(x.getItemId(), x.getPrice()));
                for (auto sale: s.getCurrentScene().getChildrenRecursively<Sale>()) {
                    sale->setItemId(-1);
                    sale->setName("");
                    sale->updatePrice(100);
                }
            }
        });
        inventory.bind(sale);
    }
}

void setupGuide(WorldState &state, Canvas &guide, Canvas& mainMenu) {
    auto label = std::make_shared<Label>("guide", "Guide\n");
    label->turnOn(COLOR_YELLOW);
    guide.bind(label);

    std::string guideText =
            "STONKS GAME is a exchange trading simulator. In this game you\n"
            "should buy items and sell them with new price.\n\n"

            "Сontrol binds:\n"
            "Arrow up   - go to higher button\n"
            "Arrow down - go to lower button\n"
            "Enter      - interaction with button\n"
            "Arrow right/left - change price of item\n"
            "KEY 1      - switch to main menu\n"
            "KEY 2      - switch to stock exchange\n"
            "KEY 3      - switch to your inventory\n";
    auto guideForPlayer = std::make_shared<Label>("guide", guideText);
    guide.bind(guideForPlayer);

    // button for travelling from guide to main Menu
    auto butGdMn = std::make_shared<Button>("back", 0, state, [&](WorldState& state, Button &x) {
        state.changeCurrentScene(mainMenu);
    });
    guide.bind(butGdMn);
}

