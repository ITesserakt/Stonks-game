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
#include "game_widgets/dsl/DSL.h"
#include "widgets/dsl/DSL.h"

using namespace widget;
using namespace widget::setup;
using namespace widget::implicit;
using namespace std::string_literals;
using namespace std::chrono_literals;

void setupMainMenu(WorldState &state, Canvases &scenes) {
    setup::auto_index<Button> guard;

    canvas{scenes[MainMenu]}.append(label{"game name", "Stonks Game\n", color(COLOR_YELLOW)},
            button{"play", guard, command<SceneChangeCommand>(state, scenes[GameField])},
            button{"settings", guard, command<SceneChangeCommand>(state, scenes[Settings])},
            button{"guide", guard, command<SceneChangeCommand>(state, scenes[Guide])},
            button{"statistics", guard, command<SceneChangeCommand>(state, scenes[Statistics])},
            button{"quit", guard, command<ShutdownCommand>(state)});
}

void setupGameField(WorldState &state, Canvases &scenes) {
    auto balanceUpdate = [&]() {
        return std::string("Balance: ") + std::to_string(state.getPlayer().getBalance()) + std::string(" $");
    };

    canvas{scenes[GameField]}.append(
            group{"management", label{"stocks", "Game field\n"} << color(COLOR_YELLOW),
                    label{"money_amount", "Balance: \n", withUpdate(100ms, balanceUpdate)},
                    many<Purchase>(Config::current().worldSize) << command<PurchaseCommand>(self<Purchase>{}, state),
                    message_box{"win_message", "You have won!", Center, hide(true)}},
            group{"statistics",
                    shared_graphic{"Price", "$", "t", {30, 20}, state}});
}

void setupInventory(WorldState &state, Canvases &scenes) {
    canvas{scenes[Inventory]}.append(
            group{"management",
                  label{"inv", "Inventory\n", color(COLOR_YELLOW)},
                  many<Sale>(Config::current().activePreset().inventorySize)
                        << command<SaleCommand>(self<Sale>{}, state)},
            group{"statistics",
                    shared_graphic{"Price", "$", "t", {30, 20}, state}});
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
    auto_index<Button> guard;
    auto               toggle = [toggled = false]() mutable {
        toggled = !toggled;
        return toggled;
    };
    auto constructLevel = [&state, &guard](std::size_t idx) {
        auto preset = Config::current().presets[idx];
        return std::move(*button{
                preset.name, guard, command_fn{[idx, &state] {
                    Config::modify([idx](ConfigData &data) { data.difficulty = idx; });
                    ShutdownCommand(state).act();
                }}}.constructed);
    };
    auto restartConfigFn = [] {
        Config::modify([](ConfigData &data) { data = ConfigData{}; });
        std::filesystem::remove("../share/save.json");
        std::filesystem::remove("../share/statistic.json");
        exit(0);
    };
    auto restartSaveFn = [] {
        std::filesystem::remove("../share/save.json");
        std::filesystem::remove("../share/statistic.json");
        exit(0);
    };

    canvas{scenes[Settings]}.append(
            group{"initial", label{"title", "Settings\n", color(COLOR_YELLOW)},
                    button{"reset_config", guard,
                            command<HideCommand>(search<Group>("initial"), true) +
                                    command<HideCommand>(search<Group>("restart_config"), false) +
                                    command<ChangeActiveWidgetCommand>(state, Direction::DOWN)},
                    button{"remove_save", guard,
                            command<HideCommand>(search<Group>("initial"), true) +
                                    command<HideCommand>(search<Group>("restart_save"), false) +
                                    command<ChangeActiveWidgetCommand>(state, Direction::DOWN)},
                    button{"Difficulties", guard, command<HideCommand>(search<Group>("difficulties"), toggle)},
                    group{"difficulties", many<Button>(Config::current().presets.size(), constructLevel) << hide(true)},
                    button{"back", guard, command<SceneChangeCommand>(state, scenes[MainMenu])}},
            group{"restart_config",
                    message_box{"restart_message",
                            "Do you want to restart game\n"
                            "to apply config changes?\n"
                            "It will cause save delete.",
                            SpecialPosition::Special},
                    button{"yes", guard, command_fn{restartConfigFn}},
                    button{"no", guard,
                            command<HideCommand>(search<Group>("initial"), false) +
                                    command<HideCommand>(search<Group>("restart_config"), true) +
                                    command<ChangeActiveWidgetCommand>(state, Direction::UP, 2)}}
                    << hide(true),
            group{"restart_save",
                    message_box{"restart_message",
                            "Are you sure to delete save file?\n"
                            "It cannot be restored",
                            SpecialPosition::Special},
                    button{"yes", guard, command_fn{restartSaveFn}},
                    button{"no", guard,
                            command<HideCommand>(search<Group>("initial"), true) +
                                    command<HideCommand>(search<Group>("restart_save"), false) +
                                    command<ChangeActiveWidgetCommand>(state, Direction::UP, 2)}}
                    << hide(true));
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
