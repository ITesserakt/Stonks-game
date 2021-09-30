#pragma once

#include <utility>

#include "PositionedWidget.h"
#include "ColorWidget.h"

class WorldState;

class HoverableWidget : public virtual PositionedWidget, public virtual ColorWidget {
protected:
    WorldState& state;
    std::function<void(WorldState&, HoverableWidget&)> todo;
    int tabIndex;

public:
    // We are on button
    void onHoverStart(){
        turnOn(COLOR_GREEN);

        for (auto x: getChildrenRecursively<ColorWidget>())
            x->turnOn(COLOR_GREEN);
    }

    // We go out of button
    void onHoverEnd() {
        turnOff();

        for (auto x: getChildrenRecursively<ColorWidget>())
            x->turnOff();
    }

    void click() {
        todo(state, *this);
    }

    void setName(std::string newName) { name = std::move(newName); }

    int getTabIndex() const { return tabIndex; }

    explicit HoverableWidget(int index, WorldState &state, std::function<void(WorldState&, HoverableWidget&)> f)
            : state(state), todo(std::move(f)), tabIndex(index) {}
};
