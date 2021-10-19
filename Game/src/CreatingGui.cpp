#include "CreatingGui.h"
#include "GameWidgets/Purchase.h"
#include "widgets/MessageBox.h"
#include "WorldState.h"
#include <iostream>

void createCanvas(const std::string &name, const Align &al, Canvases &scenes,
                  WorldState &state,
                  std::function<void(WorldState &, Canvases &)> setupCanvas) {
    scenes.push_back(std::make_shared<Canvas>(name, al));
    setupCanvas(state, scenes);
}

void setupMainMenu(WorldState &state, Canvases &scenes) {
    auto label1 = std::make_shared<Label>("game name", "STONKS GAME\n");
    label1->turnOn(COLOR_YELLOW);
    auto butPl = std::make_shared<Button>("play", 0, state, [&](WorldState &state, Button &x) {
        state.changeCurrentScene(*scenes[SceneNames::GameField].get());
    });
    auto butSt = std::make_shared<Button>("settings", 1, state, [&](WorldState &state, Button &x) {
        state.changeCurrentScene(*scenes[SceneNames::Settings].get());
    });
    auto butGd = std::make_shared<Button>("guide", 2, state, [&](auto &_, auto &x) {
        state.changeCurrentScene(*scenes[SceneNames::Guide].get());
    });
    auto butQ = std::make_shared<Button>("quit", 3, state, [&](WorldState &s, auto &x) {
        quitGame();
    });
    scenes[SceneNames::MainMenu]->bind(label1);
    scenes[SceneNames::MainMenu]->bind(butPl);
    scenes[SceneNames::MainMenu]->bind(butSt);
    scenes[SceneNames::MainMenu]->bind(butGd);
    scenes[SceneNames::MainMenu]->bind(butQ);
}

void setupGameField(WorldState &state, Canvases &scenes) {
    auto label2 = std::make_shared<Label>("stocks", "Game field\n");
    label2->turnOn(COLOR_YELLOW);
    scenes[SceneNames::GameField]->bind(label2);

    auto balance = std::make_shared<Label>("Money Amount", "Balance: \n");
    scenes[SceneNames::GameField]->bind(balance);

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
        scenes[SceneNames::GameField]->bind(purch);
    }

    auto winMessage = std::make_shared<MessageBox>("Win Message", "You have win!");
    winMessage->hide(true);
    scenes[SceneNames::GameField]->bind(winMessage);
}

void setupInventory(WorldState &state, Canvases &scenes) {
    auto label = std::make_shared<Label>("inventory", "Inventory\n");
    scenes[SceneNames::Inventory]->bind(label);
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
        scenes[SceneNames::Inventory]->bind(sale);
    }
}

void setupGuide(WorldState &state, Canvases &scenes) {
    auto label = std::make_shared<Label>("guide", "Guide\n");
    label->turnOn(COLOR_YELLOW);
    scenes[SceneNames::Guide]->bind(label);

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
    scenes[SceneNames::Guide]->bind(guideForPlayer);

    // button for travelling from guide to main Menu
    auto butGdMn = std::make_shared<Button>("back", 0, state, [&](WorldState &state, Button &x) {
        state.changeCurrentScene(*scenes[SceneNames::MainMenu]);
    });
    scenes[SceneNames::Guide]->bind(butGdMn);
}

void setupSettings(WorldState &state, Canvases &scenes) {
    auto label = std::make_shared<Label>("guide", "Settings\n");
    label->turnOn(COLOR_YELLOW);

    auto restartMessage = std::make_shared<MessageBox>("configRestart",
                                                       "Do you want to restart game\n"
                                                       "to apply config changes?");

    auto yes = std::make_shared<Button>("yes", 2, state, [=](auto &s, auto &x) { quitGame(); });
    auto no = std::make_shared<Button>("no", 3, state);
    auto butStMn = std::make_shared<Button>("back", 1, state);

    auto butRt = std::make_shared<Button>("reset\nconfig", 0, state, [=](WorldState &s, Button &x) {
        Config::refresh();
        restartMessage->hide(false);
        yes->hide(false);
        no->hide(false);
        s.getCurrentScene().changeActiveWidget(Direction::DOWN, 2);
        x.hide();
        butStMn->hide();
    });

    restartMessage->hide();
    yes->hide();
    no->hide();

    no->applyAction([=](WorldState &s, auto &x) {
        butRt->hide(false);
        butStMn->hide(false);
        s.getCurrentScene().changeActiveWidget(Direction::UP, 2);
        restartMessage->hide();
        yes->hide();
        x.hide();
    });

    butStMn->applyAction([=](WorldState &s, auto &w) {
       restartMessage->hide();
       yes->hide();
       no->hide();
       s.changeCurrentScene(*scenes[SceneNames::MainMenu]);
    });

    scenes[SceneNames::Settings]->bind(label);
    scenes[SceneNames::Settings]->bind(restartMessage);
    scenes[SceneNames::Settings]->bind(yes);
    scenes[SceneNames::Settings]->bind(no);
    scenes[SceneNames::Settings]->bind(butRt);
    scenes[SceneNames::Settings]->bind(butStMn);
}

