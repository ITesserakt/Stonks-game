#include <ncurses.h>
#include <cpp-terminal/terminal.h>
#include "GUI.h"

void console_gui::__detail::NCursesBootstrap::bootstrap() const {
    initscr();
    start_color();
    noecho();                       // Removes characters when typed
    curs_set(0);                    // Removes cursor
    keypad(stdscr, true);

    if (getmaxx(stdscr) < 80 || getmaxy(stdscr) < 24) {
        endwin();
        throw std::runtime_error("Your terminal should be bigger or equal to 80x24 size");
    }
}

void console_gui::__detail::TerminalBootstrap::bootstrap() const {
    Term::Terminal term(true);
    term.save_screen();
    int rows, cols;

    if (!term.get_term_size(rows, cols) || cols < 80 || rows < 24) {
        term.restore_screen();
        throw std::runtime_error("Your terminal should be bigger or equal to 80x24 size");
    }
}