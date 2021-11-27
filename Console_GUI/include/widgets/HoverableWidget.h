#pragma once

#include <utility>

#include "ColorWidget.h"
#include "PositionedWidget.h"

class Command;

class HoverableWidget : public virtual PositionedWidget, public virtual ColorWidget {
protected:
    std::unique_ptr<Command> todo;
    int tabIndex;

public:
    template <typename C>
    explicit HoverableWidget(int index, C &&cmd)
        : tabIndex(index),
          todo(std::make_unique<C>(std::forward<C>(cmd))) {}

    template <typename C>
    void applyAction(C &&command) {
        todo = std::make_unique<C>(std::forward<C>(command));
    }

    // We are on button
    void onHoverStart();

    // We go out of button
    void onHoverEnd();

    void click();

    void setName(std::string newName);

    int getTabIndex() const;

    template <typename C>
    bool hasCommand() { return dynamic_cast<C *>(todo.get()); }
};
