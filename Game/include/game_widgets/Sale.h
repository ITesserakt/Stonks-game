//
// Created by tesserakt on 27.11.2021.
//

#pragma once

#include "Command.h"
#include "game_widgets/TradeButton.h"
#include "GameObject.h"

class Sale : public TradeButton {
    friend struct SaleCommand;

private:
    int              index;
    GameObject::Cost newPrice = 0;

public:
    template <typename C>
    explicit Sale(int tabIndex, int index, C &&cmd)
        : Widget(""), ColorWidget(""), HoverableWidget(tabIndex, Command::noop()), index(index) {
        applyAction(std::forward<C>(cmd));
    }

    explicit Sale(int tabIndex, int index) : Sale(tabIndex, index, Command::noop()) {}

    GameObject::Cost currentPrice();

    void growPrice(GameObject::Cost value);

    UISize getSize() override;

    void show() override;

    void update(GameObject &&item);
};