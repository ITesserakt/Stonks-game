#pragma once

#include <queue>
#include "GUIEvent.h"
#include "Widget.h"

class GUIEvent;

class EventSequence
{
private:
    std::queue<std::unique_ptr<GUIEvent>> internal;

public:
    template <typename T>
    void andThen(T event)
    {
        internal.push(std::make_unique(std::move(event)));
    }

    template <typename T>
    void moveTo(T &destination)
    {
        std::unique_ptr<GUIEvent> item;
        while (!internal.empty())
        {
            item = std::move(internal.back());
            internal.pop();
            destination.push(std::move(item));
        }
    }

    static EventSequence empty()
    {
        return {};
    }
};