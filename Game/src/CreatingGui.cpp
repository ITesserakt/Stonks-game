#include <random>
#include <filesystem>

#include "CreatingGui.h"
#include "WorldState.h"
#include "Statistics.h"
#include "commands/HideCommand.h"
#include "commands/PurchaseCommand.h"
#include "commands/SaleCommand.h"
#include "commands/SceneChangeCommand.h"
#include "commands/ShutdownCommand.h"
#include "game_widgets/Purchase.h"
#include "game_widgets/Sale.h"
#include "widgets/Graphic.h"
#include "widgets/Group.h"
#include "widgets/MessageBox.h"

void setupMainMenu(WorldState &state, Canvases &scenes) {
    auto label = std::make_shared<Label>("game name", "STONKS GAME\n");
    auto butPl = std::make_shared<Button>("play", 0, SceneChangeCommand(state, scenes[SceneNames::GameField]));
    auto butSt = std::make_shared<Button>("settings", 1, SceneChangeCommand(state, scenes[SceneNames::Settings]));
    auto butGd = std::make_shared<Button>("guide", 2, SceneChangeCommand(state, scenes[SceneNames::Guide]));
    auto butStat = std::make_shared<Button>("statistics", 3, SceneChangeCommand(state, scenes[SceneNames::Statistics]));
    auto butQ = std::make_shared<Button>("quit", 4, ShutdownCommand(state));

    label->turnOn(COLOR_YELLOW);
    scenes[SceneNames::MainMenu]->bind(label);
    scenes[SceneNames::MainMenu]->bind(butPl);
    scenes[SceneNames::MainMenu]->bind(butSt);
    scenes[SceneNames::MainMenu]->bind(butGd);
    scenes[SceneNames::MainMenu]->bind(butStat);
    scenes[SceneNames::MainMenu]->bind(butQ);
}

void setupGameField(WorldState &state, Canvases &scenes) {
    auto label = std::make_shared<Label>("stocks", "Game field\n");
    label->turnOn(COLOR_YELLOW);
    scenes[SceneNames::GameField]->bind(label);

    auto balance = std::make_shared<Label>("Money Amount", "Balance: \n");
    balance->setRegularNameChanging(std::chrono::milliseconds(100), [&](){
              return std::string("Balance: ") + std::to_string(state.getPlayer().getBalance());
           });
    scenes[SceneNames::GameField]->bind(balance);

    for (int i = 0; i < state.getWorld().getSlots().size(); i++) {
        auto purch = std::make_shared<Purchase>(i, Command::noop());
        purch->applyAction(PurchaseCommand(purch, state));
        scenes[SceneNames::GameField]->bind(purch);
    }

    auto winMessage = std::make_shared<MessageBox>("Win Message", "You have win!");
    winMessage->hide(true);
    scenes[SceneNames::GameField]->bind(winMessage);

}

void setupInventory(WorldState &state, Canvases &scenes) {
    auto label = std::make_shared<Label>("inventory", "Inventory\n");
    label->turnOn(COLOR_YELLOW);
    scenes[SceneNames::Inventory]->bind(label);

    for (unsigned long i = 0; i < state.getPlayer().getInventorySize(); i++) {
        auto sale = std::make_shared<Sale>(i, Command::noop());
        sale->applyAction(SaleCommand(sale, state));
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
    auto butGdMn = std::make_shared<Button>("back", 0, SceneChangeCommand(state, scenes[SceneNames::MainMenu]));
    scenes[SceneNames::Guide]->bind(butGdMn);
}

void setupSettings(WorldState &state, Canvases &scenes) {
    auto label = std::make_shared<Label>("guide", "Settings\n");
    label->turnOn(COLOR_YELLOW);

    auto butRt = std::make_shared<Button>("reset\nconfig", 0);

    std::vector<std::shared_ptr<Label>> levelNames;
    auto levels = std::make_shared<Button>("difficulties", 1,
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

    auto butStMn = std::make_shared<Button>("back", 2, SceneChangeCommand(state, scenes[SceneNames::MainMenu]));

    auto initialGroup = std::make_shared<Group>("Initial");
    initialGroup->bind(label);
    initialGroup->bind(butRt);
    initialGroup->bind(levels);
    //for (const auto &item : levelNames)
        //initialGroup->bind(item);
    initialGroup->bind(butStMn);
    scenes[SceneNames::Settings]->bind(initialGroup);

    auto restartMessage = std::make_shared<MessageBox>("configRestart",
                                                       "Do you want to restart game\n"
                                                       "to apply config changes?", SpecialPosition::Special);

    auto yes = std::make_shared<Button>("yes", 3);
    yes->applyAction(StateCommand::fromFunction(state, [](WorldState &s){
                         std::__fs::filesystem::remove("../share/config.json");
                     }).then(ShutdownCommand(state)));
    auto no = std::make_shared<Button>("no", 4);

    auto groupForRestart = std::make_shared<Group>("Restart");
    groupForRestart->bind(restartMessage);
    groupForRestart->bind(yes);
    groupForRestart->bind(no);
    groupForRestart->hide(true);
    scenes[SceneNames::Settings]->bind(groupForRestart);

    butRt->applyAction(
            HideCommand(groupForRestart, false)
                    .then(HideCommand(initialGroup))
                    .then(StateCommand::fromFunction(state, [](WorldState &state) {         // for shifting
                        state.getCurrentScene().changeActiveWidget(Direction::DOWN, 1);    // cursor
                    })));

    no->applyAction(
            HideCommand(initialGroup, false)
                    .then(HideCommand(groupForRestart))
                    .then(StateCommand::fromFunction(state, [](WorldState &state) {
                        state.getCurrentScene().changeActiveWidget(Direction::UP, 2);
                    })));

    butStMn->applyAction(
                    SceneChangeCommand(state, scenes[SceneNames::MainMenu]));

}

void setupStatistics(WorldState &state, Canvases &scenes) {
    auto leftGroup = std::make_shared<Group>("Management");

    auto label = std::make_shared<Label>("Statistics", "STONKS-TISTICS\n");
    label->turnOn(COLOR_YELLOW);
    auto butStMn = std::make_shared<Button>("Back", 0);
    butStMn->applyAction(SceneChangeCommand(state, scenes[SceneNames::MainMenu]));
    leftGroup->bind(label);
    leftGroup->bind(butStMn);

    auto rightGroup = std::make_shared<Group>("Statistics");
    auto amountOfPurchases = std::make_shared<Label>("Stat1", "Amount of purchases: 0\n");
    amountOfPurchases->setRegularNameChanging(std::chrono::seconds {1}, [&]() -> std::string{
             return std::string("Amount of purchases: ")
               + std::to_string(Stat::Statistic::getValueByName("buyItem")) + std::string("\n");
    });
    auto TestGraphic = std::make_shared<Graphic>("Test", "y", "t",
                                                 UISize{30, 10}, [&]() -> int {
              std::random_device dev;
              std::mt19937 rng(dev());
              std::uniform_int_distribution<int> distribution(0, 10);
              return distribution(rng);
              // TODO: return round(state::statistics->getVAlue);
            });
    rightGroup->bind(amountOfPurchases);
    rightGroup->bind(TestGraphic);

    scenes[SceneNames::Statistics]->bind(leftGroup);
    scenes[SceneNames::Statistics]->bind(rightGroup);
}
