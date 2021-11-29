//
// Created by tesserakt on 28.11.2021.
//

#pragma once

#include <functional>

#include "Command.h"

template <typename W>
class HideCommand : public virtual WidgetCommand<W>, public virtual CloneCommand<HideCommand<W>> {
private:
    std::function<bool()> hideFn;

public:
    template <typename F>
    explicit HideCommand(std::shared_ptr<W> sender, F &&f) : WidgetCommand<W>(sender), hideFn(std::forward<F>(f)) {}

    explicit HideCommand(std::shared_ptr<W> sender, bool hide = true)
        : WidgetCommand<W>(sender),
          hideFn([hide] { return hide; }) {}

    void act() override {
        this->sender->hide(hideFn());
    }
};