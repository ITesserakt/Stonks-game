#include "thread"
#include "Frontend.h"
#include <ncurses.h>

// Libraries for presentation
#include "widgets/Button.h"
#include "widgets/PlainText.h"
#include "Canvas.h"

void presentationOfGui() {
    std::shared_ptr<Canvas>canvas = std::make_shared<Canvas>("Main Menu", Centered);
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
                                      find(list.begin(), list.end(), canvas->whoOnHover()));
            if (index - 1 >= 1) {
                std::dynamic_pointer_cast<Button>(list[index])->onHoverEnd();
                std::dynamic_pointer_cast<Button>(list[index - 1])->onHoverStart();
            }
        }
        else if (key == KEY_DOWN) {
            auto list = canvas->getChildren();
            int index = std::distance(list.begin(),
                                      find(list.begin(), list.end(), canvas->whoOnHover()));
            if (index + 1 < list.size()) {
                std::dynamic_pointer_cast<Button>(list[index])->onHoverEnd();
                std::dynamic_pointer_cast<Button>(list[index + 1])->onHoverStart();
            }
        }
        else if (key == '\n') {
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

    //auto f = Frontend();

    presentationOfGui();

    endwin();
}