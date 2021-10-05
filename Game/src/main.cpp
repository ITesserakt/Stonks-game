#include "CreatingGui.h"
#include "FramePainter.h"
#include "EventHandler.h"
#include "WorldState.h"
#include "Config.h"
#include "widgets/MessageBox.h"
#include <ncurses.h>
#include <unistd.h>
#include <thread>

int main() {
    setupCurses();
    checkWindowSize();

    canvases scenes;
    createCanvas("MainMenu", Centered, scenes);
    createCanvas("GameField", Left, scenes);
    createCanvas("Inventory", Left, scenes);
    createCanvas("Guide", Left, scenes);
    createCanvas("Settings", Centered, scenes);
    WorldState state(*scenes[SceneNames::MainMenu], Config::botsAmount, Config::debug);

    setupMainMenu(state, scenes);
    setupGameField(state, scenes);
    setupInventory(state, scenes);
    setupGuide(state, scenes);
    setupSettings(state, scenes);

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