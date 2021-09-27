#pragma once

#include "widgets/Button.h"
#include "widgets/Purchase.h"
#include "widgets/Label.h"

#define AMOUNT_OF_MENUS 3

inline std::vector<std::shared_ptr<Canvas>> createCanvases() {
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

    auto Inventory = std::make_shared<Canvas>("Inventory", Left);
    auto label3 = std::make_shared<Label>("inventory", "Inventory\n");
    Inventory->bind(label3);
    label3->turnOn(COLOR_YELLOW);

    return {MainMenu, GameField, Inventory};
}
