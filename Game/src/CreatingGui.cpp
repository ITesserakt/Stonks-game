#include "CreatingGui.h"

#include <filesystem>
#include <random>

#include "Config.h"
#include "Statistics.h"
#include "WorldState.h"
#include "commands/HideCommand.h"
#include "commands/PurchaseCommand.h"
#include "commands/SaleCommand.h"
#include "commands/SceneChangeCommand.h"
#include "commands/ShutdownCommand.h"
#include "game_widgets/Purchase.h"
#include "game_widgets/Sale.h"

using namespace std::chrono_literals;
using namespace std::string_literals;
using namespace widget;

void setupMainMenu(WorldState &state, Canvases &scenes) {
    setup::auto_index<button> guard;

    canvas{scenes[MainMenu]}.append(label{"game name", "Stonks Game\n", setup::color(COLOR_YELLOW)},
            button{"play", guard, setup::command<SceneChangeCommand>(state, scenes[GameField])},
            button{"settings", guard, setup::command<SceneChangeCommand>{state, scenes[Settings]}},
            button{"guide", guard, setup::command<SceneChangeCommand>{state, scenes[Guide]}},
            button{"statistics", guard, setup::command<SceneChangeCommand>{state, scenes[Statistics]}},
            button{"quit", guard, setup::command<ShutdownCommand>{state}});
}

void setupGameField(WorldState &state, Canvases &scenes) {
    auto balanceUpdate = [&] { return "Balance: "s + std::to_string(state.getPlayer().getBalance()) + "$"; };

    canvas{scenes[GameField]}.append(label{"stocks", "Game field\n", setup::color(COLOR_YELLOW)},
            label{"balance", "Balance:", setup::withUpdate(100ms, std::move(balanceUpdate))},
            many<Purchase>{Config::current().worldSize} << setup::sender_command<PurchaseCommand>(state),
            message_box{"win_message", "You have win!", Center, widget::setup::hide(true)});
}

void setupInventory(WorldState &state, Canvases &scenes) {
    canvas{scenes[Inventory]}.append(label{"inv", "Inventory\n", widget::setup::color(COLOR_YELLOW)},
            many<Sale>(Config::current().activePreset().inventorySize) << setup::sender_command<SaleCommand>(state));
}

void setupGuide(WorldState &state, Canvases &scenes) {
    constexpr auto guideText = "STONKS GAME is a exchange trading simulator. In this game you\n"
                               "should buy items and sell them with new price.\n\n"

                               "Сontrol binds:\n"
                               "Arrow up   - go to higher button\n"
                               "Arrow down - go to lower button\n"
                               "Enter      - interaction with button\n"
                               "Arrow right/left - change price of item\n"
                               "KEY 1      - switch to main menu\n"
                               "KEY 2      - switch to stock exchange\n"
                               "KEY 3      - switch to your inventory\n";

    canvas{scenes[Guide]}.append(label{"guide", "Guide\n", setup::color(COLOR_YELLOW)}, label{"guide_text", guideText},
            button{"back", 0, setup::command<SceneChangeCommand>(state, scenes[SceneNames::MainMenu])});
}

void setupSettings(WorldState &state, Canvases &scenes) {
    auto label = std::make_shared<Label>("guide", "Settings\n");
    label->turnOn(COLOR_YELLOW);

    int  butIndex = 0;
    auto butRt    = std::make_shared<Button>("reset config", butIndex++);
    auto butSR    = std::make_shared<Button>("remove save", butIndex++);

    auto levelNames = std::make_shared<Group>("Difficulties");
    auto levels     = std::make_shared<Button>(
            "difficulties", butIndex++, Command::fromFunction([isLevelsExpanded = false, levelNames]() mutable {
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

    auto butStMn =
            std::make_shared<Button>("back", butIndex++, SceneChangeCommand(state, scenes[SceneNames::MainMenu]));

    auto initialGroup = std::make_shared<Group>("Initial");
    initialGroup->bind(label);
    initialGroup->bind(butRt);
    initialGroup->bind(butSR);
    initialGroup->bind(levels);
    initialGroup->bind(levelNames);
    initialGroup->bind(butStMn);
    scenes[SceneNames::Settings]->bind(initialGroup);

    auto restartMessage = std::make_shared<MessageBox>("configRestart",
            "Do you want to restart game\n"
            "to apply config changes?",
            SpecialPosition::Special);

    auto yes = std::make_shared<Button>("yes", butIndex++, Command::fromFunction([] {
        Config::modify([](ConfigData &data) { data = ConfigData{}; });
    }).then(ShutdownCommand(state)));
    auto no  = std::make_shared<Button>("no", butIndex++);

    auto groupForRestart = std::make_shared<Group>("Restart");
    groupForRestart->bind(restartMessage);
    groupForRestart->bind(yes);
    groupForRestart->bind(no);
    groupForRestart->hide(true);
    scenes[SceneNames::Settings]->bind(groupForRestart);

    auto saveDelete = std::make_shared<MessageBox>("save_remove",
            "Are you sure to delete save file?\n"
            "It cannot be restored",
            SpecialPosition::Special);

    auto yesSR              = std::make_shared<Button>("yes", butIndex++, Command::fromFunction([] {
        std::filesystem::remove("../share/save.json");
        exit(0);
                 }));
    auto noSR               = std::make_shared<Button>("no", butIndex++);
    auto groupForRemoveSave = std::make_shared<Group>("Save remove");
    groupForRemoveSave->bind(saveDelete);
    groupForRemoveSave->bind(yesSR);
    groupForRemoveSave->bind(noSR);
    groupForRemoveSave->hide();
    scenes[SceneNames::Settings]->bind(groupForRemoveSave);

    butRt->applyAction(HideCommand(*groupForRestart, false) | HideCommand(*initialGroup) |
                       StateCommand::fromFunction(state, [](WorldState &state) {          // for shifting
                           state.getCurrentScene().changeActiveWidget(Direction::DOWN, 1);// cursor
                       }));

    butSR->applyAction(HideCommand(*groupForRemoveSave, false) | HideCommand(*initialGroup) |
                       StateCommand::fromFunction(state, [](WorldState &state) {          // for shifting
                           state.getCurrentScene().changeActiveWidget(Direction::DOWN, 1);// cursor
                       }));

    no->applyAction(HideCommand(*initialGroup, false) | HideCommand(*groupForRestart) |
                    StateCommand::fromFunction(state,
                            [](WorldState &state) { state.getCurrentScene().changeActiveWidget(Direction::UP, 2); }));

    noSR->applyAction(HideCommand(*initialGroup, false) | HideCommand(*groupForRemoveSave) |
                      StateCommand::fromFunction(state,
                              [](WorldState &state) { state.getCurrentScene().changeActiveWidget(Direction::UP, 2); }));

    butStMn->applyAction(SceneChangeCommand(state, scenes[SceneNames::MainMenu]));
}

void setupStatistics(WorldState &state, Canvases &scenes) {
    auto stonksUpdate = [&state] {
        return "Stonks: "s +
               std::to_string(state.getPlayer().getBalance() - Config::current().activePreset().initialMoney) + "$\n";
    };
    auto purchasesUpdate = [] {
        return "Amount of purchases: "s + std::to_string(Stat::Statistic::getValueByName("amountOfPurchases")) + "\n";
    };
    auto itemUpdate = [] {
        return "Amount of items in world: "s + std::to_string(Stat::Statistic::getValueByName("amountOfItemsInWorld")) +
               "\n";
    };
    auto stocksUpdate = [&] {
        using namespace Stat;
        return std::abs(Statistic::getValueByName("sellItem") - Statistic::getValueByName("buyItem"));
    };

    canvas{scenes[SceneNames::Statistics]}.append(
            group{"management", label{"statistics", "STONK-tistics\n", setup::color(COLOR_YELLOW)},
                    button{"back", 0, setup::command<SceneChangeCommand>(state, scenes[SceneNames::MainMenu])}},

            group{"stats", label{"stat1", "Stonks: 0\n", setup::withUpdate(1s, stonksUpdate)},
                    label{"stat2", "Amount of purchases: \n", setup::withUpdate(1s, purchasesUpdate)},
                    label{"stat3", "Amount of items in world: \n", setup::withUpdate(1s, itemUpdate)},
                    graphic{"stocks", "δ", "t", {30, 10}, stocksUpdate}});
}
