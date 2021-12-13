//
// Created by tesserakt on 27.11.2021.
//

#pragma once

#include "Command.h"
#include "GameObject.h"
#include "game_widgets/TradeButton.h"

class Sale : public TradeButton {
    friend struct SaleCommand;

private:
    GameObject::Cost                        newPrice = 0;

public:
    template <typename C>
    explicit Sale(int index, C &&cmd) : Widget(""), ColorWidget(""), HoverableWidget(index, Command::noop()) {
        applyAction(std::forward<C>(cmd));
    }

    explicit Sale(int index) : Sale(index, Command::noop()) {}

    GameObject::Cost currentPrice();

    void growPrice(GameObject::Cost value);

    UISize getSize() override;

    void show() override;

    void update(GameObject &&item);
};