#include "CreatingGui.h"
#include "Purchase.h"
#include "widgets/Widget.h"
#include <iostream>
#include "WorldState.h"

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
        throw std::runtime_error("Your terminal should be bigger or equal "
                                 "to 80x24 size\n");
    }
}

void setupMainMenu(WorldState &state, Canvas &mainMenu, Canvas &gameField) {
    auto label1 = std::make_shared<Label>("game name", "STONKS GAME\n");
    label1->turnOn(COLOR_YELLOW);
    auto butPl =
            std::make_shared<Button>("play", 0, state,
                                     [&](WorldState &state, Button &x) {
                                         state.changeCurrentScene(gameField);
                                     });
    auto butSt =
            std::make_shared<Button>("settings", 1, state,
                                     [](auto &_, auto &x) {});
    auto butQ =
            std::make_shared<Button>("quit", 2, state,
                                     [&](WorldState &s, auto &x) {
                                         std::cout << "See you later ;)"
                                                   << std::endl;
                                         clear();
                                         endwin();
                                         exit(0);
                                     });
    mainMenu.bind(label1);
    mainMenu.bind(butPl);
    mainMenu.bind(butSt);
    mainMenu.bind(butQ);
}

void setupGameField(WorldState &state, Canvas &gameField) {
    auto label2 = std::make_shared<Label>("stocks", "Game field\n");
    label2->turnOn(COLOR_YELLOW);
    gameField.bind(label2);

    std::vector<std::shared_ptr<Purchase>> container;
    for (unsigned long i = 0; i < state.getWorld().getSlots().size(); i++) {
        container.emplace_back(new Purchase(i, state, [&](WorldState& s, Purchase& x) {
            if (x.getItemId() != (unsigned int)-1) {
                s.getPlayer().buyItem(s.getWorld().takeItem(x.getItemId()));
                x.setCost(0);
                x.setName("");
            }
        }));
        gameField.bind(container[i]);
    }
}

void setupInventory(Canvas &inventory) {
    auto label3 = std::make_shared<Label>("inventory", "Inventory\n");
    inventory.bind(label3);
    label3->turnOn(COLOR_YELLOW);
}
