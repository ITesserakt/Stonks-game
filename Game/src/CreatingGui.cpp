#include "Config.h"// for changing current difficulty
#include <filesystem>
#include <random>

#include "commands/ChangeActiveWidgetCommand.h"
#include "commands/HideCommand.h"
#include "commands/PurchaseCommand.h"
#include "commands/SaleCommand.h"
#include "commands/SceneChangeCommand.h"
#include "commands/ShutdownCommand.h"
#include "CreatingGui.h"
#include "game_widgets/dsl/DSL.h"
#include "game_widgets/Purchase.h"
#include "game_widgets/Sale.h"
#include "Statistics.h"
#include "widgets/dsl/DSL.h"
#include "WorldState.h"

using namespace widget;
using namespace widget::setup;
using namespace widget::implicit;
using namespace std::string_literals;
using namespace std::chrono_literals;

void setupMainMenu(WorldState &state, Scenes &scenes) {
    scenes[MainMenu].append(label{"game name", "Stonks Game\n", color(COLOR_YELLOW)},
            button{"play", 0,
                    command<SceneChangeCommand>(state, scenes[GameField]) + command_fn{[&state]() { state.run(); }}},
            button{"settings", 1, command<SceneChangeCommand>(state, scenes[Settings])},
            button{"guide", 2, command<SceneChangeCommand>(state, scenes[Guide])},
            button{"statistics", 3, command<SceneChangeCommand>(state, scenes[Statistics])},
            button{"quit", 4, command<ShutdownCommand>(state)});
}

void setupGameField(WorldState &state, Scenes &scenes) {
    unsigned int guard         = 2;
    auto         balanceUpdate = [&]() {
        return std::string("Balance: ") + std::to_string(state.getPlayer().getBalance()) + std::string(" $");
    };
    auto constructPurchase = [&guard](std::size_t idx) { return Purchase(guard++, idx + 1); };

    scenes[GameField].append(
            group{"management", label{"stocks", "Game field\n"} << color(COLOR_YELLOW),
                    label{"money_amount", "Balance: \n", withUpdate(100ms, balanceUpdate)},
                    button{"Go to inventory", 0} << command<SceneChangeCommand>(state, scenes[SceneNames::Inventory]),
                    button{"Go to main menu", 1} << command<SceneChangeCommand>(state, scenes[SceneNames::MainMenu]),
                    label{"empty", ""},
                    many<Purchase>(Config::current().worldSize, constructPurchase)
                            << command<PurchaseCommand>(self<Purchase>, state)},
            group{"statistics", shared_graphic{"Price", "$", "t", {30, 20}, state}, label{"empty", ""},
                    message_box{"loose_message", "You have loose.\nTry next time!", Special, hide(true)},
                    message_box{"win_message", "You have won!", Special, hide(true)}});
}

void setupInventory(WorldState &state, Scenes &scenes) {
    unsigned int guard = 2;

    scenes[Inventory].append(group{"management", label{"inv", "Inventory\n", color(COLOR_YELLOW)},
                                     button{"Go to stocks", 0, command<SceneChangeCommand>(state, scenes[GameField])},
                                     button{"Go to main menu", 1, command<SceneChangeCommand>(state, scenes[MainMenu])},
                                     label{"empty", ""},
                                     many<Sale>(Config::current().activePreset().inventorySize,
                                             [&guard](std::size_t idx) { return Sale(guard++, idx + 1); })
                                             << command<SaleCommand>(self<Sale>, state)},
            group{"statistics", shared_graphic{"Price", "$", "t", {30, 20}, state}});
}

void setupGuide(WorldState &state, Scenes &scenes) {
    constexpr auto guideText = "Stonks Game is a exchange trading simulator. In this game you\n"
                               "should buy items and sell them with new price to win.\n\n"

                               "Key binds:\n"
                               "Arrow up   - go to higher button\n"
                               "Arrow down - go to lower button\n"
                               "Enter      - interaction with button\n"
                               "Arrow right/left - change price of item\n";

    scenes[Guide].append(label{"guide", "Guide\n", setup::color(COLOR_YELLOW)}, label{"guide_text", guideText},
            button{"back", 0, setup::command<SceneChangeCommand>(state, scenes[SceneNames::MainMenu])});
}

void setupSettings(WorldState &state, Scenes &scenes) {
    unsigned int guard  = 8;
    auto         toggle = [toggled = false]() mutable {
        toggled = !toggled;
        return toggled;
    };
    auto constructLevel = [&state, &guard](std::size_t idx) {
        auto preset = Config::current().presets[idx];
        return Button(preset.name, guard++, Command::fromFunction([idx, &state] {
            Config::modify([idx](ConfigData &data) { data.difficulty = idx; });
            ShutdownCommand(state).act();
        }));
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

    scenes[Settings].append(
            group{"initial", label{"title", "Settings\n", color(COLOR_YELLOW)},
                    button{"reset config", 0,
                            command<HideCommand>(search<Group>("initial"), true) +
                                    command<HideCommand>(search<Group>("restart_config"), false) +
                                    command<ChangeActiveWidgetCommand>(state, Direction::DOWN)},
                    button{"remove save", 1,
                            command<HideCommand>(search<Group>("restart_save"), false) +
                                    command<HideCommand>(search<Group>("initial"), true) +
                                    command<ChangeActiveWidgetCommand>(state, Direction::DOWN)},
                    button{"levels", 2, command<HideCommand>(search<Group>("difficulties"), toggle)},
                    group{"difficulties", many<Button>(Config::current().presets.size(), constructLevel) << hide(true)},
                    button{"back", 100, command<SceneChangeCommand>(state, scenes[MainMenu])}},
            group{"restart_config",
                    message_box{"restart_message",
                            "Are you sure to reset config file?\n"
                            "It will cause save delete.",
                            SpecialPosition::Special},
                    button{"yes", 4, command_fn{restartConfigFn}},
                    button{"no", 5,
                            command<HideCommand>(search<Group>("initial"), false) +
                                    command<HideCommand>(search<Group>("restart_config"), true) +
                                    command<HideCommand>(search<Group>("difficulties"), true) +
                                    command<ChangeActiveWidgetCommand>(state, Direction::UP, 2)}}
                    << hide(true),
            group{"restart_save",
                    message_box{"restart_message",
                            "Are you sure to delete save file?\n"
                            "It cannot be restored",
                            SpecialPosition::Special},
                    button{"yes", 6, command_fn{restartSaveFn}},
                    button{"no", 7,
                            command<HideCommand>(search<Group>("initial"), false) +
                                    command<HideCommand>(search<Group>("restart_save"), true) +
                                    command<HideCommand>(search<Group>("difficulties"), true) +
                                    command<ChangeActiveWidgetCommand>(state, Direction::UP, 2)}}
                    << hide(true));
}

void setupStatistics(WorldState &state, Scenes &scenes) {
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

    scenes[Statistics].append(group{"management", label{"statistics", "STONKS-TISTICS\n", color(COLOR_YELLOW)},
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

Scenes::Scenes(std::initializer_list<Canvas> list) {
    for (auto &&item : list)
        scenes.emplace_back(std::make_shared<Canvas>(std::forward<Canvas>(const_cast<Canvas &&>(item))));
}

widget::canvas Scenes::operator[](SceneNames name) {
    return widget::canvas{scenes.at(static_cast<std::size_t>(name))};
}
