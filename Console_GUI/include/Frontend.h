#pragma once

#include <tbb/concurrent_queue.h>
#include "GUIEvent.h"

class Frontend {
    //TODO: here we need to specialize a facade to contact with gui

protected:
    tbb::concurrent_queue<std::unique_ptr<GUIEvent>> todo;

public:
    Frontend() = default;

    Frontend(const Frontend &) = delete;

    void flush() {
        std::unique_ptr<GUIEvent> item;
        while (todo.try_pop(item)) {
            item->action();
        }
    }

    void push(std::unique_ptr<GUIEvent> event) {
        todo.push(std::move(event));
    }
};