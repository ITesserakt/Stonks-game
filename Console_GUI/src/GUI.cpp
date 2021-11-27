#include <ncurses.h>
#include <cpp-terminal/terminal.h>
#include "GUI.h"

struct console_gui::__detail::NCursesFrontend : public Frontend {
    void init() override {
        setlocale(LC_CTYPE, "UTF-8");
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

    void dispose() override {
        endwin();
    }
};

class console_gui::__detail::TerminalFrontend : public Frontend {
private:
    Term::Terminal terminal = Term::Terminal(true, false);
public:
    void init() override {
        int rows, cols;
        if (!Term::Terminal::is_stdin_a_tty() || !Term::Terminal::is_stdout_a_tty() ||
            !terminal.get_term_size(rows, cols)) {
            throw std::runtime_error("I/O did not bind to terminal");
        }
        terminal.save_screen();

        if (cols < 80 || rows < 24) {
            terminal.restore_screen();
            throw std::runtime_error("Your terminal should be bigger or equal to 80x24 size");
        }
    }

    void dispose() override {
        terminal.restore_screen();
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