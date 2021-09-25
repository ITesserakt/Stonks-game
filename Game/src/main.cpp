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
    auto butPl = std::make_shared<Button>("play");
    auto butSt = std::make_shared<Button>("settings");
    auto butQ = std::make_shared<Button>("quit");
    canvas->bind(label);
    canvas->bind(butPl);
    canvas->bind(butSt);
    canvas->bind(butQ);
    butQ->turnOn(COLOR_WHITE);

    butSt->hide(true);

    canvas->show();
    refresh();


}

int main() {
    initscr();
    start_color();
    noecho();                       // Removes characters when typed
    curs_set(0);                    // Removes cursor
    //auto f = Frontend();

    presentationOfGui();

    getch();
    endwin();
}