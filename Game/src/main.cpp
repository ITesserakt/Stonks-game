#include "CreatingGui.h"
#include "FramePainter.h"
#include "EventHandler.h"
#include "WorldState.h"
#include "Config.h"
#include "widgets/MessageBox.h"
#include <thread>
#include "GUI.h"
#include "Debug.h"

using Frontend = console_gui::__detail::NCursesFrontend;
using namespace std::chrono_literals;

int main() {
    try {
        console_gui::init<Frontend>();

        Canvases scenes;
        WorldState state(Config::botsAmount, Config::debug);
        createCanvas("MainMenu", Centered, scenes,
                     state, setupMainMenu);
        createCanvas("GameField", Left, scenes,
                     state, setupGameField);
        createCanvas("Inventory", Left, scenes,
                     state, setupInventory);
        createCanvas("Guide", Left, scenes,
                     state, setupGuide);
        createCanvas("Settings", Centered, scenes,
                     state, setupSettings);
        state.changeCurrentScene(*scenes[SceneNames::MainMenu]);

        auto handler = EventHandler(scenes, state);

        std::thread renderThread([&]() {
            while (state.running()) {
                auto stamp = std::chrono::system_clock::now();
                clear();
                if (state.getCurrentScene() == *scenes[SceneNames::GameField]) {
                    paintGameFieldFrame(state, scenes, Config::debug);
                } else if (state.getCurrentScene() == *scenes[SceneNames::Inventory]) {
                    paintInventoryFrame(state, scenes);
                }
                state.getCurrentScene().show();
                refresh();
                auto passed = std::chrono::system_clock::now() - stamp;
                std::this_thread::sleep_for(16ms - passed);
            }
        });

        handler.startLoop();
        renderThread.join();
        state.cancelAllAI();
    } catch (const std::runtime_error &ex) {
        Debug::logger << ex.what();
    } catch (...) {
        Debug::logger << "Unknown error occurred";
    }
    console_gui::dispose<Frontend>();
}