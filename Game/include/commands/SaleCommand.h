//
// Created by tesserakt on 27.11.2021.
//

#pragma once

#include <utility>

#include "GameObjectCommand.h"
#include "StateCommand.h"
#include "game_widgets/Sale.h"

struct SaleCommand : virtual GameObjectCommand,
                     virtual StateCommand,
                     virtual UpdateCommand<Sale>,
                     virtual CloneCommand<SaleCommand> {
    explicit SaleCommand(Sale &sender, WorldState &state) : StateCommand(state), WidgetCommand(sender) {}

    void act() override {
        if (!object.has_value()) return;
        auto item = object.value();
        if (state.getWorld().couldPutInto()) {
            auto sold = state.getPlayer().sellItem(item.id, sender.currentPrice());
            if (sold == nullptr) return;
            state.getWorld().putItem(std::move(sold));
            sender.growPrice(-sender.currentPrice());
            object.reset();
            sender.clearItem();
        }
    }

    void update() override {
        if (sender.object.has_value()) object.emplace(*sender.object);
        else
            object.reset();
    }
};