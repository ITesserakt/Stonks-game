//
// Created by tesserakt on 27.11.2021.
//

#pragma once

#include <functional>

#include "Command.h"
#include "GameObject.h"
#include "widgets/HoverableWidget.h"
#include "game_widgets/TradeButton.h"

class Purchase : public TradeButton {
    friend struct PurchaseCommand;

private:
    int index;

public:
    template <typename C>
    Purchase(int tabIndex, int index, C &&cmd)
        : Widget(""), ColorWidget(""), HoverableWidget(tabIndex, Command::noop()), index(index) {
        applyAction(std::forward<C>(cmd));
    }

    explicit Purchase(int tabIndex, int index) : Purchase(tabIndex, index, Command::noop()) {}

    void update(GameObject &&item);

    UISize getSize() override;

    void show() override;
};
