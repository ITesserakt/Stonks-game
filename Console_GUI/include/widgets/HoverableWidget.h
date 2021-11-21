#pragma once

#include <utility>

#include "ColorWidget.h"
#include "Command.h"
#include "PositionedWidget.h"

class HoverableWidget : public virtual PositionedWidget, public virtual ColorWidget {
protected:
    std::unique_ptr<Command> todo;
    int tabIndex;

public:
    explicit HoverableWidget(int index, WorldState &state, std::function<void(WorldState &, HoverableWidget &)> f)
        : todo(Command::fromFunction(state, shared_from_this()->as<HoverableWidget>(), f)), tabIndex(index) {}

    template <typename C>
    explicit HoverableWidget(int index, C &&cmd)
        : tabIndex(index),
          todo(std::make_unique<std::remove_reference_t<C>>(std::forward<C>(cmd))) {}

    // We are on button
    void onHoverStart() {
        turnOn(COLOR_GREEN);

        for (auto x : getChildrenRecursively<ColorWidget>())
            x->turnOn(COLOR_GREEN);
    }

    // We go out of button
    void onHoverEnd() {
        turnOff();

        for (auto x : getChildrenRecursively<ColorWidget>())
            x->turnOff();
    }

    void click() {
        todo->act();
    }

    void setName(std::string newName) { name = std::move(newName); }

    int getTabIndex() const { return tabIndex; }
};
