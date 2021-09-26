#include "thread"
#include "Frontend.h"
#include "EventConductor.h"
#include <ncurses.h>
#include <unistd.h>

// Libraries for presentation
#include "widgets/Button.h"
#include "widgets/PlainText.h"
#include "Canvas.h"

void presentationOfGui() {
    auto canvas = std::make_shared<Canvas>("Main Menu", Centered);
    auto label = std::make_shared<PlainText>("STONKS GAME\n");
    label->turnOn(COLOR_YELLOW);
    auto butPl = std::make_shared<Button>("play", CanvasChanger);
    auto butSt = std::make_shared<Button>("settings", CanvasChanger);
    auto butQ = std::make_shared<Button>("quit", Quiter);
    canvas->bind(label);
    canvas->bind(butPl);
    canvas->bind(butSt);
    canvas->bind(butQ);
    butQ->onHoverStart();

    canvas->show();
    refresh();
    int key;
    while ((key = getch()) != 'q') {
        if (key == KEY_UP) {
            auto list = canvas->getChildren();
            int index = std::distance(list.begin(),
                                      find(list.begin(), list.end(),
                                           canvas->whoOnHover()));
            if (index - 1 >= 1) {
                std::dynamic_pointer_cast<Button>(list[index])->onHoverEnd();
                std::dynamic_pointer_cast<Button>(
                        list[index - 1])->onHoverStart();
            }
        } else if (key == KEY_DOWN) {
            auto list = canvas->getChildren();
            int index = std::distance(list.begin(),
                                      find(list.begin(), list.end(),
                                           canvas->whoOnHover()));
            if (index + 1 < list.size()) {
                std::dynamic_pointer_cast<Button>(list[index])->onHoverEnd();
                std::dynamic_pointer_cast<Button>(
                        list[index + 1])->onHoverStart();
            }
        } else if (key == '\n') {
            auto item = std::dynamic_pointer_cast<Button>(canvas->whoOnHover());
            if (item->isClickable()) { item->click(); }
        }
        canvas->show();
        refresh();
    }

}

int main() {
    initscr();
    start_color();
    noecho();                       // Removes characters when typed
    curs_set(0);                    // Removes cursor
    keypad(stdscr, true);

    if (getmaxx(stdscr) < 80 || getmaxy(stdscr) < 24) {
        endwin();
        printf("Your terminal should be bigger or equal to 80x24 size\n");
        return -1;
    }

    // Section of Gui init
    auto MainMenu = std::make_shared<Canvas>("MainMenu", Centered);
    auto label1 = std::make_shared<PlainText>("STONKS GAME\n");
    auto butQ = std::make_shared<Button>("quit", Quiter);
    MainMenu->bind(label1);
    label1->turnOn(COLOR_YELLOW);

    // Test buttons
    auto butPl = std::make_shared<Button>("play", CanvasChanger);
    auto butSt = std::make_shared<Button>("settings", CanvasChanger);
    auto space = std::make_shared<PlainText>("");
    MainMenu->bind(butPl);
    MainMenu->bind(butSt);
    MainMenu->bind(butQ);
    MainMenu->bind(space);

    auto GameField = std::make_shared<Canvas>("GameField", Left);
    auto label2 = std::make_shared<PlainText>("Game Field\n");
    GameField->bind(label2);
    label2->turnOn(COLOR_YELLOW);

    auto Inventory = std::make_shared<Canvas>("Inventory", Left);
    auto label3 = std::make_shared<PlainText>("Inventory\n");
    Inventory->bind(label3);
    label3->turnOn(COLOR_YELLOW);

    std::vector<std::shared_ptr<Canvas>> scenes = {nullptr, MainMenu, GameField,
                                                   Inventory};
    auto &current = MainMenu;
    current->firstOnHover();

    bool gameRunning = true;
    EventConductor director;
    Event game;

    std::thread guiThread([&]() {
        while (gameRunning) {
            usleep(16666);
            erase();
            current->show();
            refresh();
        }
    });

    while (director.waitEvent(game)) {
        switch (game.type) {
            case Event::key_up:
                current->changeActiveWidget(toTheTop);
                break;
            case Event::key_down:
                current->changeActiveWidget(toTheBot);
                break;
            case Event::key_enter:
                current->whoOnHover()->click();
                break;
            case Event::changeScene:
                current = scenes[game.changingScene.nextScene];
                current->firstOnHover();
                break;
            case Event::noEvent:
                break;
        }
        game = Event();
    }

    endwin();
    guiThread.join();
}