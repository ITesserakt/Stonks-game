#include "CreatingGui.h"
#include "FramePainter.h"
#include "EventHandler.h"
#include "WorldState.h"
#include "Config.h"
#include "widgets/MessageBox.h"
#include <unistd.h>
#include <thread>
#include "GUI.h"

int main() {
    console_gui::initGUI<console_gui::Terminal>();

    Canvases scenes;
    WorldState state(Config::botsAmount, Config::debug);
    createCanvas("MainMenu", Centered, scenes,
                 state,setupMainMenu);
    createCanvas("GameField", Left, scenes,
                 state,setupGameField);
    createCanvas("Inventory", Left, scenes,
                 state,setupInventory);
    createCanvas("Guide", Left, scenes,
                 state,setupGuide);
    createCanvas("Settings", Centered, scenes,
                 state,setupSettings);
    state.changeCurrentScene(*scenes[SceneNames::MainMenu]);

    auto handler = EventHandler(scenes, state);

    std::thread([&]() {
        while (state.running()) {
            usleep(16666);
            clear();
            if (state.getCurrentScene() == *scenes[SceneNames::GameField]) {
                paintGameFieldFrame(state, scenes, Config::debug);
            } else if (state.getCurrentScene() == *scenes[SceneNames::Inventory]) {
                paintInventoryFrame(state, scenes);
            }
            state.getCurrentScene().show();
            refresh();
        }
    }).detach();

    handler.startLoop();
    endwin();
}