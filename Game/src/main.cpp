#include "CreatingGui.h"
#include "FramePainter.h"
#include "Canvas.h"
#include "EventHandler.h"
#include "WorldState.h"
#include "Config.h"
#include "widgets/MessageBox.h"
#include <ncurses.h>
#include <unistd.h>
#include <thread>

int main(int argc, char **argv) {
    bool debugFlag = false;
    if (argc >= 2)
        debugFlag = (std::string("-d") == std::string(argv[1]));
    debugFlag |= Config::debug;
    setupCurses();
    checkWindowSize();

    auto mainMenu = std::make_shared<Canvas>("MainMenu", Centered);
    auto settings = std::make_shared<Canvas>("Settings", Centered);
    auto gameField = std::make_shared<Canvas>("GameField", Left);
    auto inventory = std::make_shared<Canvas>("Inventory", Left);
    auto guide = std::make_shared<Canvas>("Guide", Left);
    canvases scenes = {mainMenu, gameField, inventory, guide, settings};
    WorldState state(*scenes[SceneNames::MainMenu], Config::botsAmount, debugFlag);

    setupMainMenu(state, scenes);
    setupGameField(state, *gameField);
    setupInventory(state, *inventory);
    setupGuide(state, *guide, *mainMenu);
    setupSettings(state, scenes);

    auto handler = EventHandler(scenes, state);

    std::thread([&]() {
        while (state.running()) {
            usleep(16666);
            clear();
            if (state.getCurrentScene() == *scenes[SceneNames::GameField]) {
                paintGameFieldFrame(state, scenes, debugFlag);
            } else if (state.getCurrentScene() == *scenes[SceneNames::Inventory].get()) {
                paintInventoryFrame(state, scenes);
            }
            state.getCurrentScene().show();
            refresh();
        }
    }).detach();

    handler.startLoop();
    endwin();
}