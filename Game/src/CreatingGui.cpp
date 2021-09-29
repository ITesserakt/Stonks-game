#include <World.h>
#include "CreatingGui.h"
#include "widgets/Purchase.h"
#include "widgets/Widget.h"

int findCanvsID(std::vector<std::shared_ptr<Canvas>> list,
                std::shared_ptr<Canvas> can) {
    return std::distance(list.begin(),
                         std::find(list.begin(), list.end(), can));
}

std::vector<std::shared_ptr<Canvas>>
createCanvases(std::shared_ptr<Player> guy, World &world) {
    std::vector<std::shared_ptr<Canvas>> res;

    auto MainMenu = std::make_shared<Canvas>("MainMenu", Centered);
    res.push_back(MainMenu);
    auto label1 = std::make_shared<Label>("game name", "STONKS GAME\n");

    MainMenu->bind(label1);
    label1->turnOn(COLOR_YELLOW);

    // Test buttons
    auto space = std::make_shared<Label>("space", "");
    MainMenu->bind(space);

    auto GameField = std::make_shared<Canvas>("GameField", Left);
    res.push_back(GameField);
    auto label2 = std::make_shared<Label>("stocks", "Game field\n");
    GameField->bind(label2);
    label2->turnOn(COLOR_YELLOW);

    // FIXME that's govnocod :(
    // Purchase
    auto purch1 = std::make_shared<Purchase>(0, [&](Purchase& x) {
        guy->buyItem(world.takeItem(x.getItemId()));
    });
    GameField->bind(purch1);
    auto purch2 = std::make_shared<Purchase>(1, [&](Purchase& x) {
        guy->buyItem(world.takeItem(x.getItemId()));
    });
    GameField->bind(purch2);
    auto purch3 = std::make_shared<Purchase>(2, [&](Purchase& x) {
        guy->buyItem(world.takeItem(x.getItemId()));
    });
    GameField->bind(purch3);
    auto purch4 = std::make_shared<Purchase>(3, [&](Purchase& x) {
        guy->buyItem(world.takeItem(x.getItemId()));
    });
    GameField->bind(purch4);
    auto purch5 = std::make_shared<Purchase>(4, [&](Purchase& x) {
        guy->buyItem(world.takeItem(x.getItemId()));
    });
    GameField->bind(purch5);
    auto purch6 = std::make_shared<Purchase>(5, [&](Purchase& x) {
        guy->buyItem(world.takeItem(x.getItemId()));
    });
    GameField->bind(purch6);
    auto purch7 = std::make_shared<Purchase>(6, [&](Purchase& x) {
        guy->buyItem(world.takeItem(x.getItemId()));
    });
    GameField->bind(purch7);
    auto purch8 = std::make_shared<Purchase>(7, [&](Purchase& x) {
        guy->buyItem(world.takeItem(x.getItemId()));
    });
    GameField->bind(purch8);
    auto purch9 = std::make_shared<Purchase>(8, [&](Purchase& x) {
        guy->buyItem(world.takeItem(x.getItemId()));
    });
    GameField->bind(purch9);
    auto purch10 = std::make_shared<Purchase>(9, [&](Purchase& x) {
        guy->buyItem(world.takeItem(x.getItemId()));
    });
    GameField->bind(purch10);

    auto Inventory = std::make_shared<Canvas>("Inventory", Left);
    res.push_back(Inventory);
    auto label3 = std::make_shared<Label>("inventory", "Inventory\n");
    Inventory->bind(label3);
    label3->turnOn(COLOR_YELLOW);

    return res;
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