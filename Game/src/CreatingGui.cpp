#include <World.h>
#include "CreatingGui.h"
#include "widgets/Purchase.h"
#include "widgets/Widget.h"

std::vector<std::shared_ptr<Canvas>> createCanvases(std::shared_ptr<Player> guy, const World& world) {
    std::vector<std::shared_ptr<Canvas>> res;

    auto MainMenu = std::make_shared<Canvas>("MainMenu", Centered);
    auto label1 = std::make_shared<Label>("game name", "STONKS GAME\n");
    auto butQ = std::make_shared<Button>("quit", Quiter, 2);
    MainMenu->bind(label1);
    label1->turnOn(COLOR_YELLOW);

    // Test buttons
    auto butPl = std::make_shared<Button>("play", CanvasChanger, 0);
    auto butSt = std::make_shared<Button>("settings", CanvasChanger, 1);
    auto space = std::make_shared<Label>("space", "");
    MainMenu->bind(butPl);
    MainMenu->bind(butSt);
    MainMenu->bind(butQ);
    MainMenu->bind(space);

    auto GameField = std::make_shared<Canvas>("GameField", Left);
    auto label2 = std::make_shared<Label>("stocks", "Game field\n");
    GameField->bind(label2);
    label2->turnOn(COLOR_YELLOW);

    // Purchase
    // TODO fix bug with >1 purchase button show();
    auto purch1 = std::make_shared<Purchase>(0);
    GameField->bind(purch1);
    auto purch2 = std::make_shared<Purchase>(1);
    GameField->bind(purch2);
    auto purch3 = std::make_shared<Purchase>(2);
    GameField->bind(purch3);
    auto purch4 = std::make_shared<Purchase>(3);
    GameField->bind(purch4);
    auto purch5 = std::make_shared<Purchase>(4);
    GameField->bind(purch5);
    auto purch6 = std::make_shared<Purchase>(5);
    GameField->bind(purch6);
    auto purch7 = std::make_shared<Purchase>(6);
    GameField->bind(purch7);
    auto purch8 = std::make_shared<Purchase>(7);
    GameField->bind(purch8);
    auto purch9 = std::make_shared<Purchase>(8);
    GameField->bind(purch9);
    auto purch10= std::make_shared<Purchase>(9);
    GameField->bind(purch10);

    auto Inventory = std::make_shared<Canvas>("Inventory", Left);
    auto label3 = std::make_shared<Label>("inventory", "Inventory\n");
    Inventory->bind(label3);
    label3->turnOn(COLOR_YELLOW);

    return {MainMenu, GameField, Inventory};
}

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
