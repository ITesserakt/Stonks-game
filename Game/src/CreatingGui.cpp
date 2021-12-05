#include "Config.h"// for changing current difficulty
#include <filesystem>
#include <jsoncons_ext/jsonpath/json_query.hpp>// for changing current difficulty
#include <random>

#include "CreatingGui.h"
#include "Statistics.h"
#include "WorldState.h"
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
    balance->setRegularNameChanging(std::chrono::milliseconds(100), [&]() {
        return std::string("Balance: ") + std::to_string(state.getPlayer().getBalance()) + std::string(" $");
    });
    scenes[SceneNames::GameField]->bind(balance);

    for (int i = 0; i < Config::current().worldSize; i++) {
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

    int butIndex = 0;
    auto butRt = std::make_shared<Button>("reset\nconfig", butIndex++);

    auto levelNames = std::make_shared<Group>("Difficulties");
    auto levels = std::make_shared<Button>("difficulties", butIndex++,
                                           Command::fromFunction([isLevelsExpanded = false, levelNames]() mutable {
                                               levelNames->hide(isLevelsExpanded);
                                               isLevelsExpanded = !isLevelsExpanded;
                                           }));

    int number = 0;
    for (const auto &level : Config::current().presets) {
        auto levelLabel = std::make_shared<Button>("level " + std::to_string(number), butIndex++);
        levelLabel->applyAction(Command::fromFunction([number]() {
                                    Config::modify([number](ConfigData &data) { data.difficulty = number; });
                                }).then(ShutdownCommand(state)));
        levelNames->bind(levelLabel);
        number++;
    }
    levelNames->hide(true);

    auto butStMn = std::make_shared<Button>("back", butIndex++, SceneChangeCommand(state, scenes[SceneNames::MainMenu]));

    auto initialGroup = std::make_shared<Group>("Initial");
    initialGroup->bind(label);
    initialGroup->bind(butRt);
    initialGroup->bind(levels);
    initialGroup->bind(levelNames);
    initialGroup->bind(butStMn);
    scenes[SceneNames::Settings]->bind(initialGroup);

    auto restartMessage = std::make_shared<MessageBox>("configRestart",
                                                       "Do you want to restart game\n"
                                                       "to apply config changes?",
                                                       SpecialPosition::Special);

    auto yes = std::make_shared<Button>("yes", butIndex++);
    yes->applyAction(Command::fromFunction([] {
                         Config::modify([](ConfigData &data) { data = ConfigData{}; });
                     }).then(ShutdownCommand(state)));
    auto no = std::make_shared<Button>("no", butIndex++);

    auto groupForRestart = std::make_shared<Group>("Restart");
    groupForRestart->bind(restartMessage);
    groupForRestart->bind(yes);
    groupForRestart->bind(no);
    groupForRestart->hide(true);
    scenes[SceneNames::Settings]->bind(groupForRestart);

    butRt->applyAction(
            HideCommand(groupForRestart, false)
                    .then(HideCommand(initialGroup))
                    .then(StateCommand::fromFunction(state, [](WorldState &state) {    // for shifting
                        state.getCurrentScene().changeActiveWidget(Direction::DOWN, 1);// cursor
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
    auto Stonksity = std::make_shared<Label>("Stat1", "Stonksity: 0\n");
    Stonksity->setRegularNameChanging(std::chrono::seconds{1}, [&]() -> std::string {
        using namespace std::string_literals;
        return "Stonksity: "s + std::to_string(state.getPlayer().getBalance() - Config::current().activePreset().initialMoney) + "$\n";
    });
    auto amountOfPurchases = std::make_shared<Label>("Stat1", "Amount of purchases: 0\n");
    amountOfPurchases->setRegularNameChanging(std::chrono::seconds{1}, [&]() -> std::string {
        using namespace std::string_literals;
        return "Amount of purchases: "s + std::to_string(Stat::Statistic::getValueByName("amountOfPurchases")) + "\n";
    });
    auto amountOfItemsInWorld = std::make_shared<Label>("Stat2", "Amount of items in world: \n");
    amountOfItemsInWorld->setRegularNameChanging(std::chrono::seconds{1}, [&]() -> std::string {
        using namespace std::string_literals;
        return "Amount of items in world: "s + std::to_string(Stat::Statistic::getValueByName("amountOfItemsInWorld")) + "\n";
    });
    auto deltaGraphic = std::make_shared<Graphic>("Test", "δ", "t",
                                                  UISize{30, 10}, [&]() -> int {
                                                      using namespace Stat;
                                                      return abs(Statistic::getValueByName("sellItem") -
                                                                 Statistic::getValueByName("buyItem"));
                                                  });
    auto graphicExplain = std::make_shared<Label>("Explaining deltaGrphic", "Global price change");
    rightGroup->bind(Stonksity);
    rightGroup->bind(amountOfPurchases);
    rightGroup->bind(amountOfItemsInWorld);
    rightGroup->bind(deltaGraphic);
    rightGroup->bind(graphicExplain);

    scenes[SceneNames::Statistics]->bind(leftGroup);
    scenes[SceneNames::Statistics]->bind(rightGroup);
}
