//
// Created by tesserakt on 27.11.2021.
//

#pragma once

#include "GameObjectCommand.h"
#include "StateCommand.h"
#include "game_widgets/Sale.h"

struct SaleCommand : virtual GameObjectCommand, virtual StateCommand, virtual UpdateCommand<Sale> {
    explicit SaleCommand(Sale &sender, WorldState &state)
        : StateCommand(state), WidgetCommand(sender) {}

    void act() override {
        if (!object.has_value()) return;
        auto item = object.value();
        if (state.getWorld().couldPutInto()) {
            state.getWorld().putItem(state.getPlayer().sellItem(item.id, sender.currentPrice()));
            sender.growPrice(-sender.currentPrice());
            object.reset();
            sender.clearItem();
        }
    }

    void update() override {
        if (sender.object.has_value())
            object.emplace(*sender.object);
        else
            object.reset();
    }
};