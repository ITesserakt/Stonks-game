//
// Created by tesserakt on 27.11.2021.
//

#pragma once

#include <functional>

#include "Command.h"
#include "GameObject.h"
#include "optional.hpp"
#include "widgets/HoverableWidget.h"

class Purchase : public HoverableWidget {
    friend struct PurchaseCommand;

private:
    std::experimental::optional<GameObject> object;

public:
    template <typename C>
    Purchase(int index, C &&cmd) : Widget(""), ColorWidget(""), HoverableWidget(index, Command::noop()) {
        applyAction(std::forward<C>(cmd));
    }

    explicit Purchase(int index) : Purchase(index, Command::noop()) {}

    void update(GameObject &&item);
    void clearItem();

    bool isObjectExist() { return object.has_value(); }
    GameObject::Id getItemId() { return object->id; }

    UISize getSize() override;

    void show() override;
};
