//
// Created by tesserakt on 28.11.2021.
//

#pragma once

#include <functional>

#include "Command.h"

class HideCommand
    : public virtual WidgetCommand<PositionedWidget>,
      public virtual CloneCommand<HideCommand> {
private:
    std::function<bool()> hideFn;

public:
    template <typename F>
    explicit HideCommand(PositionedWidget &sender, F &&f) : WidgetCommand(sender), hideFn(std::forward<F>(f)) {}

    explicit HideCommand(PositionedWidget &sender, bool hide = true)
        : WidgetCommand(sender), hideFn([hide] { return hide; }) {}

    void act() override {
        this->sender.hide(hideFn());
    }
};