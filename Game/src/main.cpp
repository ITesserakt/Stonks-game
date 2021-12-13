#include "Config.h"
#include "CreatingGui.h"
#include "Debug.h"
#include "EventHandler.h"
#include "FramePainter.h"
#include "GUI.h"
#include "GameLoading.h"
#include "Statistics.h"
#include "WorldState.h"
#include "widgets/MessageBox.h"
#include "widgets/dsl/DSL.h"
#include <thread>

using Frontend = console_gui::NCurses;
using namespace std::chrono_literals;

void close(bool withFailure) {
    console_gui::dispose<Frontend>();
    if (withFailure) std::cout << "Exception occurred. See log.txt for details" << std::endl;
}

int main() {
    bool       failure = false;
    WorldState state   = loadWorldState();
    loadStatistic();
    std::set_terminate([] { close(true); });
    try {
        console_gui::init<Frontend>();

        Canvases scenes = {std::make_shared<Canvas>("MainMenu", Centered),
                std::make_shared<Canvas>("GameField", Vsplit), std::make_shared<Canvas>("Inventory", Vsplit),
                std::make_shared<Canvas>("Guide", Left), std::make_shared<Canvas>("Settings", Centered),
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
        std::thread               renderThread([&]() {
            while (state.running()) {
                auto stamp = std::chrono::system_clock::now();
                clear();
                if (state.getCurrentScene() == *scenes[SceneNames::GameField]) {
                    paintGameFieldFrame(state, scenes, Config::current().debug);
                }
                else if (state.getCurrentScene() == *scenes[SceneNames::Inventory]) {
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

    saveGameData(state);
    close(failure);
}