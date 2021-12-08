#include "Config.h"
#include "CreatingGui.h"
#include "Debug.h"
#include "EventHandler.h"
#include "FramePainter.h"
#include "GUI.h"
#include "Statistics.h"
#include "WorldState.h"
#include "widgets/MessageBox.h"
#include <thread>

using Frontend = console_gui::NCurses;
using namespace std::chrono_literals;

constexpr auto savePath = "../share/save.json";

WorldState loadWorldState() {
    std::ifstream in{savePath};
    std::optional<WorldState> state;

    try {
        state.emplace(jsoncons::decode_json<WorldState>(in));
    } catch (...) {
        state.emplace(Config::current().botsAmount, Config::current().debug);
    }
    return std::move(state.value());
}

void loadStatistic() {
    std::ifstream in(Stat::pathForSave);
    try {
        jsoncons::json j = jsoncons::json::parse(in);
        Stat::Counter() = j["counter"].as<Stat::Counter>();
        Stat::ItemStat() = j["items"].as<Stat::ItemStat>();
    } catch (...) {

    }
}

void close(bool withFailure) {
    console_gui::dispose<Frontend>();
    if (withFailure)
        std::cout << "Exception occurred. See log.txt for details" << std::endl;
}

int main() {
    bool failure = false;
    WorldState state = loadWorldState();
    loadStatistic();
    std::set_terminate([] { close(true); });
    try {
        console_gui::init<Frontend>();

        Canvases scenes = {
                std::make_shared<Canvas>("MainMenu", Centered),
                std::make_shared<Canvas>("GameField", Vsplit),
                std::make_shared<Canvas>("Inventory", Left),
                std::make_shared<Canvas>("Guide", Left),
                std::make_shared<Canvas>("Settings", Centered),
                std::make_shared<Canvas>("Statistics", Vsplit)};
        setupMainMenu(state, scenes);
        setupGameField(state, scenes);
        setupInventory(state, scenes);
        setupGuide(state, scenes);
        setupSettings(state, scenes);
        setupStatistics(state, scenes);

        state.changeCurrentScene(*scenes[SceneNames::MainMenu]);

        auto handler = EventHandler(scenes, state);

        std::chrono::milliseconds sleepTime{static_cast<int>(1.0 / Config::current().maxFPS * 1000)};
        std::thread renderThread([&]() {
            while (state.running()) {
                auto stamp = std::chrono::system_clock::now();
                clear();
                if (state.getCurrentScene() == *scenes[SceneNames::GameField]) {
                    paintGameFieldFrame(state, scenes, Config::current().debug);
                } else if (state.getCurrentScene() == *scenes[SceneNames::Inventory]) {
                    paintInventoryFrame(state, scenes);
                }
                state.getCurrentScene().show();
                refresh();
                auto passed = std::chrono::system_clock::now() - stamp;
                std::this_thread::sleep_for(sleepTime - passed);
            }
        });

        handler.startLoop();
        renderThread.join();
        state.cancelAllAI();
    } catch (const std::exception &ex) {
        failure = true;
        Debug::logger << ex.what();
    } catch (...) {
        failure = true;
        Debug::logger << "Unknown error occurred";
    }
    // TODO replace to destructor
    std::ofstream out(savePath);
    jsoncons::encode_json_pretty(state, out);
    out.close();
    out.open(Stat::pathForSave);
    jsoncons::json j;
    j["counter"] = Stat::Counter();
    j["items"] = Stat::ItemStat();
    jsoncons::encode_json_pretty(j, out);
    close(failure);
}
