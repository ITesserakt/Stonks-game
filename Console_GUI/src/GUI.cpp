#include <ncurses.h>
#include <cpp-terminal/window.hpp>
#include <locale>

#include "GUI.h"

struct console_gui::__detail::NCursesFrontend : public Frontend {
    void init() override {
        std::locale::global(std::locale(""));
        initscr();
        start_color();
        noecho();   // Removes characters when typed
        curs_set(0);// Removes cursor
        keypad(stdscr, true);

        if (getmaxx(stdscr) < 80 || getmaxy(stdscr) < 24) {
            endwin();
            throw std::runtime_error("Your terminal should be bigger or equal to 80x24 size");
        }
    }

    void dispose() override {
        endwin();
    }
};

class console_gui::__detail::TerminalFrontend : public Frontend {
private:
    Term::Terminal terminal = Term::Terminal(true, true, false);

public:
    void init() override {
        int rows, cols;
        if (!Term::is_stdin_a_tty() || !Term::is_stdout_a_tty() ||
            !Term::get_term_size(rows, cols)) {
            throw std::runtime_error("I/O did not bind to terminal");
        }
        Term::save_screen();

        if (cols < 80 || rows < 24) {
            Term::restore_screen();
            throw std::runtime_error("Your terminal should be bigger or equal to 80x24 size");
        }
    }

    void dispose() override {
        Term::restore_screen();
    }
};

static console_gui::__detail::TerminalFrontend terminal;

template<>
void console_gui::init<console_gui::NCurses>() {
    __detail::NCursesFrontend().init();
}

template<>
void console_gui::init<console_gui::Terminal>() {
    terminal.init();
}

template<>
void console_gui::dispose<console_gui::NCurses>() {
    __detail::NCursesFrontend().dispose();
}

template<>
void console_gui::dispose<console_gui::Terminal>() {
    terminal.dispose();
}