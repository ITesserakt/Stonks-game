#include <iomanip>
#include <range/v3/all.hpp>

#include "FramePainter.h"
#include "game_widgets/Purchase.h"
#include "widgets/MessageBox.h"
#include "game_widgets/Sale.h"

void paintGameFieldFrame(WorldState &state, Canvases &scenes, const bool &debugFlag) {
    auto slots = state.getWorld().getSlots();
    auto purches = scenes[SceneNames::GameField]->getChildrenRecursively<Purchase>();

    for (const auto &purchase : purches)
        purchase->clearItem();

    for (auto next : ranges::views::zip(slots, purches)) {
        auto slot = next.first;
        auto purch = next.second;
        auto item = state.getWorld().viewItem(slot);
        std::stringstream ss;
        if (debugFlag)
            ss << item << ", profitness: " << state.getWorld().getProfitness(slot);
        else
            ss << item.fullName() << (item.timesSold > 0 ? " *" : "");
        purch->setName(ss.str());
        purch->update(std::move(item));
    }
    if (state.getPlayer().getBalance() > Config::activePreset.winCondition) {
        scenes[SceneNames::GameField]->getChildWithName("Win Message")->as<MessageBox>()->hide(false);
        state.shutdown();
    }
}

void paintInventoryFrame(WorldState &state, Canvases &scenes) {
    auto sales = scenes[SceneNames::Inventory]->getChildrenRecursively<Sale>();
    auto items = state.getPlayer().getSlots();

    for (auto context : ranges::views::zip(items, sales)) {
        auto itemId = context.first;
        auto sale = context.second;

        std::stringstream ss;
        auto item = state.getPlayer().viewItem(itemId);
        ss << item.fullName() << ", cost: $" << item.cost;
        sale->setName(ss.str());
        sale->update(std::move(item));
    }
}
