#include <iostream>
#include <random>

#include "CreatingGui.h"
#include "Debug.h"
#include "widgets/Graphic.h"
#include "WorldState.h"
#include "commands/SceneChangeCommand.h"
#include "commands/ShutdownCommand.h"
#include "widgets/Group.h"
#include "widgets/MessageBox.h"

void setupMainMenu(WorldState &state, Canvases &scenes) {
    auto label1 = std::make_shared<Label>("game name", "STONKS GAME\n");
    auto butPl = std::make_shared<Button>("play", 0, SceneChangeCommand(state, scenes[SceneNames::GameField]));
    auto butSt = std::make_shared<Button>("settings", 1, SceneChangeCommand(state, scenes[SceneNames::Settings]));
    auto butGd = std::make_shared<Button>("guide", 2, SceneChangeCommand(state, scenes[SceneNames::Guide]));
    auto butQ = std::make_shared<Button>("quit", 3, ShutdownCommand(state));

    label1->turnOn(COLOR_YELLOW);
    scenes[SceneNames::MainMenu]->bind(label1);
    scenes[SceneNames::MainMenu]->bind(butPl);
    scenes[SceneNames::MainMenu]->bind(butSt);
    scenes[SceneNames::MainMenu]->bind(butGd);
    scenes[SceneNames::MainMenu]->bind(butQ);
}

void setupGameField(WorldState &state, Canvases &scenes) {
    auto label2 = std::make_shared<Label>("stocks", "Game field\n");
    auto balance = std::make_shared<Label>("Money Amount", "Balance: \n");

    label2->turnOn(COLOR_YELLOW);
    scenes[SceneNames::GameField]->bind(label2);
    scenes[SceneNames::GameField]->bind(balance);

    for (unsigned long i = 0; i < state.getWorld().getSlots().size(); i++) {
        auto purch = std::make_shared<Button>("aboba", i, Command::noop());
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
        auto sale = std::make_shared<Button>("aboba", i, Command::noop());
        scenes[SceneNames::Inventory]->bind(sale);
    }
    auto TestGraphic = std::make_shared<Graphic>("Test", "y", "x",
                                                 UISize{20, 10}, [&]() -> int{
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<int> distribution(1, 5);
        return distribution(rng);
        // TODO: return round(state::statistics->getVAlue);
    });
    scenes[SceneNames::Inventory]->bind(TestGraphic);
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
    auto butGdMn = std::make_shared<Button>("back", 0, SceneChangeCommand(state, scenes[SceneNames::MainMenu]));
    scenes[SceneNames::Guide]->bind(butGdMn);
}

void setupSettings(WorldState &state, Canvases &scenes) {
    auto label = std::make_shared<Label>("guide", "Settings\n");
    label->turnOn(COLOR_YELLOW);

    auto restartMessage = std::make_shared<MessageBox>("configRestart",
                                                       "Do you want to restart game\n"
                                                       "to apply config changes?");

    auto yes = std::make_shared<Button>("yes", 3, ShutdownCommand(state));
    auto no = std::make_shared<Button>("no", 4);
    auto butStMn = std::make_shared<Button>("back", 2, SceneChangeCommand(state, scenes[SceneNames::MainMenu]));
    std::vector<std::shared_ptr<Label>> levelNames;
    auto levels = std::make_shared<Button>(
            "difficulties", 1,
            StateCommand::fromFunction(state, [isLevelsExpanded = false](WorldState &s) mutable {
                isLevelsExpanded = !isLevelsExpanded;
                for (int i = 0; i < Config::presets.size(); i++) {
                    auto level = s.getCurrentScene().getChildWithName("level" + std::to_string(i));
                    level->as<PositionedWidget>()->hide(isLevelsExpanded);
                }
            }));

    int i = 0;
    for (const auto &level : Config::presets) {
        auto levelLabel = std::make_shared<Label>("level" + std::to_string(i), level.name);
        levelLabel->hide();
        levelNames.push_back(levelLabel);
        i++;
    }

    auto butRt = std::make_shared<Button>("reset\nconfig", 0, Command::noop());

    restartMessage->hide();
    yes->hide();
    no->hide();

    scenes[SceneNames::Settings]->bind(label);
    scenes[SceneNames::Settings]->bind(restartMessage);
    scenes[SceneNames::Settings]->bind(yes);
    scenes[SceneNames::Settings]->bind(no);
    scenes[SceneNames::Settings]->bind(butRt);
    scenes[SceneNames::Settings]->bind(levels);
    for (const auto &item : levelNames)
        scenes[SceneNames::Settings]->bind(item);
    scenes[SceneNames::Settings]->bind(butStMn);
}
