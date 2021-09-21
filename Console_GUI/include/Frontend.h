#pragma once

#include <queue>
#include "GUIEvent.h"
#include <curses.h>
#include <iostream>

class Frontend {
    //TODO: here we need to specialize a facade to contact with gui

protected:
    std::queue<std::unique_ptr<GUIEvent>> todo;

public:
    Frontend() {
        std::cout << "here" << std::endl;
        mvprintw(10, 10, "aboba");
    }

    Frontend(const Frontend &) = delete;

    void flush() {
        std::unique_ptr<GUIEvent> item;
        while (!todo.empty()) {
            todo.back()->action();
            todo.pop();
        }
    }

    void push(std::unique_ptr<GUIEvent> event) {
        todo.push(std::move(event));
    }
};