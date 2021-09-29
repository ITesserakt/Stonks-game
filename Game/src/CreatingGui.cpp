#include "World.h"
#include "CreatingGui.h"
#include "Purchase.h"
#include "widgets/Widget.h"
#include <iostream>

#define EXTRA_SLOTS 2

int findCanvsID(std::vector<std::shared_ptr<Canvas>> list,
                std::shared_ptr<Canvas> can) {
    return std::distance(list.begin(),
                         std::find(list.begin(), list.end(), can));
}

std::vector<std::shared_ptr<Canvas>>
createCanvases(std::shared_ptr<Player> guy, World &world, Canvas* current) {
    std::vector<std::shared_ptr<Canvas>> res;

    auto MainMenu = std::make_shared<Canvas>("MainMenu", Centered);
    res.push_back(MainMenu);
    auto label1 = std::make_shared<Label>("game name", "STONKS GAME\n");
    label1->turnOn(COLOR_YELLOW);
    MainMenu->bind(label1);

    auto GameField = std::make_shared<Canvas>("GameField", Left);
    res.push_back(GameField);
    auto label2 = std::make_shared<Label>("stocks", "Game field\n");
    GameField->bind(label2);
    label2->turnOn(COLOR_YELLOW);

    // TODO make ChangeCanvaser it doesn't work
    // Possible variants: Create group of canvases and ...
    auto butPl = std::make_shared<Button>("play", 0, [&](auto& x) {
    });
    MainMenu->bind(butPl);
    auto butSt = std::make_shared<Button>("settings", 1, [](auto& x) {});
    MainMenu->bind(butSt);
    auto butQ = std::make_shared<Button>("quit", 2, [&](auto &x) {
        endwin();
        std::cout << "See you later ;)" << std::endl;
        exit(0);
    });
    MainMenu->bind(butQ);


    std::vector<std::shared_ptr<Purchase>> container;
    container.reserve(world.getSlots().size());
    for (unsigned long i = 0; i < world.getSlots().size() + EXTRA_SLOTS; i++) {
        container.emplace_back(new Purchase(i, [&](HoverableWidget& x) {
            if (x.as<Purchase>()->getItemId() != (unsigned int)-1 &&
                    guy->getBalance() >= world.viewItem(x.as<Purchase>()->getItemId()).cost) {
                guy->buyItem(world.takeItem(x.as<Purchase>()->getItemId()));
                x.as<Purchase>()->setCost(0);
                x.as<Purchase>()->setName("");
            }
            }));
        GameField->bind(container[i]);
    }

    auto Inventory = std::make_shared<Canvas>("Inventory", Left);
    res.push_back(Inventory);
    auto label3 = std::make_shared<Label>("inventory", "Inventory\n");
    Inventory->bind(label3);
    label3->turnOn(COLOR_YELLOW);

    std::vector<std::shared_ptr<Sale>> sellContainer;
    sellContainer.reserve(world.getSlots().size());
    for (unsigned long i = 0; i < world.getSlots().size(); i++) {
        sellContainer.emplace_back(new Sale(i, [&](HoverableWidget& x) {
            //guy->Gamer::sellItem(x.getTabIndex(), 100);
            //x.setName(guy->viewItem(x.getItemId()).fullName());
            x.setName("");
        }));
        Inventory->bind(sellContainer[i]);
    }

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
