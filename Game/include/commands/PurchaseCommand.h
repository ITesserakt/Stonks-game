//
// Created by tesserakt on 27.11.2021.
//

#pragma once

#include <utility>

#include "GameObjectCommand.h"
#include "StateCommand.h"
#include "game_widgets/Purchase.h"

struct PurchaseCommand
    : virtual GameObjectCommand,
      virtual StateCommand,
      virtual UpdateCommand<Purchase>,
      virtual CloneCommand<PurchaseCommand> {
    explicit PurchaseCommand(std::shared_ptr<Purchase> sender, WorldState &state)
        : StateCommand(state), WidgetCommand(std::move(sender)) {}

    void act() override {
        if (!object.has_value()) return;
        auto item = object.value();
        if (state.getPlayer().couldBuy() && state.getPlayer().getBalance() > state.getWorld().viewItem(item.id).cost) {
            auto bought = state.getWorld().takeItem(item.id);
            if (bought == nullptr) return;
            state.getPlayer().buyItem(std::move(bought));
            object.reset();
            sender->clearItem();
        }
    }

    void update() override {
        if (sender->object.has_value())
            object.emplace(*sender->object);
        else
            object.reset();
    }
};