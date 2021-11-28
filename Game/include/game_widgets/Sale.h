//
// Created by tesserakt on 27.11.2021.
//

#pragma once

#include "Command.h"
#include "GameObject.h"
#include "optional.hpp"
#include "widgets/HoverableWidget.h"

class Sale : public HoverableWidget {
    friend struct SaleCommand;

private:
    GameObject::Cost newPrice = 0;
    std::experimental::optional<GameObject> object;

public:
    template <typename C>
    explicit Sale(int index, C &&cmd)
        : Widget(""),
          ColorWidget(""),
          HoverableWidget(index, Command::noop()) {
        applyAction(std::forward<C>(cmd));
    }

    GameObject::Cost currentPrice();

    void growPrice(GameObject::Cost value);

    UISize getSize() override;

    void show() override;

    void update(GameObject &&item);

    void clearItem();
};