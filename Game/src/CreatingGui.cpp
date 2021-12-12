#include "Config.h"// for changing current difficulty
#include <filesystem>
#include <random>

#include "CreatingGui.h"
#include "Statistics.h"
#include "WorldState.h"
#include "commands/ChangeActiveWidgetCommand.h"
#include "commands/HideCommand.h"
#include "commands/PurchaseCommand.h"
#include "commands/SaleCommand.h"
#include "commands/SceneChangeCommand.h"
#include "commands/ShutdownCommand.h"
#include "game_widgets/Purchase.h"
#include "game_widgets/Sale.h"
#include "game_widgets/SharedGraphic.h"
#include "game_widgets/dsl/DSL.h"
#include "widgets/dsl/DSL.h"

using namespace widget;
using namespace std::string_literals;
using namespace std::chrono_literals;
using namespace widget::setup;

void setupMainMenu(WorldState &state, Canvases &scenes) {
    setup::auto_index<button> guard;

    canvas{scenes[MainMenu]}.append(label{"game name", "Stonks Game\n", color(COLOR_YELLOW)},
            button{"play", guard, command<SceneChangeCommand>(state, scenes[GameField])},
            button{"settings", guard, command<SceneChangeCommand>{state, scenes[Settings]}},
            button{"guide", guard, command<SceneChangeCommand>{state, scenes[Guide]}},
            button{"statistics", guard, command<SceneChangeCommand>{state, scenes[Statistics]}},
            button{"quit", guard, command<ShutdownCommand>{state}});
}

void setupGameField(WorldState &state, Canvases &scenes) {
    auto balanceUpdate = [&]() {
        return std::string("Balance: ") + std::to_string(state.getPlayer().getBalance()) + std::string(" $");
    };

    canvas{scenes[GameField]}.append(
            group{"management", label{"stocks", "Game field\n"} << color(COLOR_YELLOW),
                    label{"money_amount", "Balance: \n", withUpdate(100ms, balanceUpdate)},
                    many<Purchase>(Config::current().worldSize) << sender_command<PurchaseCommand>(state),
                    message_box{"win_message", "You have won!", Center, hide(true)}},
            group{"statistics"}, shared_graphic{"Price", "$", "t", {30, 20}, state});
}// FIXME There are 3 panels on the screen. Is it expected?

void setupInventory(WorldState &state, Canvases &scenes) {
    canvas{scenes[Inventory]}.append(
            group{"management", label{"inv", "Inventory\n", color(COLOR_YELLOW)},
                    many<Sale>(Config::current().activePreset().inventorySize) << sender_command<SaleCommand>(state)},
            group{"statistics"}, shared_graphic{"Price", "$", "t", {30, 20}, state});
}// FIXME left group does not work

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
            "to apply config changes?\n"
            "It will cause save delete.",
            SpecialPosition::Special);

    auto yes = std::make_shared<Button>("yes", butIndex++, Command::fromFunction([] {
        Config::modify([](ConfigData &data) { data = ConfigData{}; });
        std::filesystem::remove("../share/save.json");
        std::filesystem::remove("../share/statistic.json");
        exit(0);
    }));
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
        std::filesystem::remove("../share/statistic.json");
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
                       ChangeActiveWidgetCommand(state, Direction::DOWN));

    butSR->applyAction(HideCommand(*groupForRemoveSave, false) | HideCommand(*initialGroup) |
                       ChangeActiveWidgetCommand(state, Direction::DOWN));

    no->applyAction(HideCommand(*initialGroup, false) | HideCommand(*groupForRestart) | HideCommand(*levelNames) |
                    ChangeActiveWidgetCommand(state, Direction::UP, 2));

    noSR->applyAction(HideCommand(*initialGroup, false) | HideCommand(*groupForRemoveSave) | HideCommand(*levelNames) |
                      ChangeActiveWidgetCommand(state, Direction::UP, 2));

    butStMn->applyAction(SceneChangeCommand(state, scenes[SceneNames::MainMenu]));
}

void setupStatistics(WorldState &state, Canvases &scenes) {
    auto stonksUpdate = [&] {
        using namespace std::string_literals;
        return "Stonksity: "s +
               std::to_string(state.getPlayer().getBalance() - Config::current().activePreset().initialMoney) + "$\n";
    };
    auto purchasesUpdate = [&] {
        using namespace std::string_literals;
        return "Amount of purchases: "s + std::to_string(Stat::Counter::getValueByName("amountOfPurchases")) + "\n";
    };
    auto itemsUpdate = [&] {
        using namespace std::string_literals;
        return "Amount of items in world: "s + std::to_string(state.getWorld().getSlots().size()) + "\n";
    };
    auto stocksUpdate = [&] {
        using namespace Stat;
        return abs(Counter::getValueByName("sellItem") - Counter::getValueByName("buyItem"));
    };
    auto timeUpdate = [] { return std::string("Time in game:            ") + Stat::Timer::getTimeByName("GameStart"); };
    auto purchasesTimeUpdate = [] {
        return std::string("Time between purchases:  ") + Stat::Timer::getTimeByName("purchaseTimer");
    };
    auto sessionTimeUpdate = [] {
        return std::string("Time in current session: ") + Stat::Timer::getTimeByName("SessionStart");
    };

    canvas{scenes[Statistics]}.append(group{"management", label{"statistics", "STONKS-TISTICS\n", color(COLOR_YELLOW)},
                                              button{"Back", 0, command<SceneChangeCommand>(state, scenes[MainMenu])}},
            group{"statistics", label{"stat1", "Stonks: 0\n", color(COLOR_WHITE), withUpdate(1s, stonksUpdate)},
                    label{"stat2", "Amount of purchases: 0", withUpdate(1s, purchasesUpdate)},
                    label{"stat3", "Amount of items in world: 0\n", withUpdate(1s, itemsUpdate)},
                    group{"stocks", graphic{"deeds", "δ", "t", {30, 10}, stocksUpdate},
                            label{"explain", "Global price change\n"}},
                    label{"stat4", "Time in game: 0", withUpdate(1s, timeUpdate)},
                    label{"stat5", "Time between purchases: 0", withUpdate(1s, purchasesTimeUpdate)},
                    label{"stat6", "Time in current session: 0", withUpdate(1s, sessionTimeUpdate)}});

    Stat::Timer::start("GameStart");
    Stat::Timer::start("SessionStart");
    Stat::Timer::noSave("SessionStart");
}
