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
public:
    template <typename C>
    Purchase(int index, C &&cmd) : Widget(""), ColorWidget(""), HoverableWidget(index, Command::noop()) {
        applyAction(std::forward<C>(cmd));
    }

    explicit Purchase(int index) : Purchase(index, Command::noop()) {}

    void update(GameObject &&item);

    UISize getSize() override;

    void show() override;
};
