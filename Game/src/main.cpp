#include "Config.h"
#include "CreatingGui.h"
#include "Debug.h"
#include "EventHandler.h"
#include "FramePainter.h"
#include "GameLoading.h"
#include "GUI.h"
#include "Statistics.h"
#include "widgets/dsl/DSL.h"
#include "widgets/MessageBox.h"
#include "WorldState.h"
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
        Scenes scenes = {Canvas("MainMenu", Centered), Canvas("GameField", Vsplit), Canvas("Inventory", Vsplit),
                Canvas("Guide", Left), Canvas("Settings", Centered), Canvas("Statistics", Vsplit)};
        setupMainMenu(state, scenes);
        setupGameField(state, scenes);
        setupInventory(state, scenes);
        setupGuide(state, scenes);
        setupSettings(state, scenes);
        setupStatistics(state, scenes);

        state.changeCurrentScene(*scenes[MainMenu]);

        auto handler = EventHandler(state);

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

        console_gui::init<Frontend>();
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