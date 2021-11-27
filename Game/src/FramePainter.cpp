#include <iomanip>
#include <range/v3/all.hpp>

#include "FramePainter.h"
#include "widgets/MessageBox.h"

void paintGameFieldFrame(WorldState &state, Canvases &scenes, const bool &debugFlag) {
    //FIXME
    //    auto slots = state.getWorld().getSlots();
    //    auto purches = scenes[SceneNames::GameField]->getChildrenRecursively<Purchase>();
    //    for (const auto &purch: purches) {
    //        purch->setName("");
    //        purch->setItemId(-1);
    //        purch->setCost(0);
    //    }
    //    for (auto[slot, purch]: ranges::views::zip(slots, purches)) {
    //        auto &item = state.getWorld().viewItem(slot);
    //        std::stringstream ss;
    //        if (debugFlag) { ss << item << ", profitness: " << state.getWorld().getProfitness(slot); }
    //        else { ss << item.fullName() << (item.timesSold > 0 ? " *" : ""); }
    //        purch->setItemId(slot);
    //        purch->setName(ss.str());
    //        purch->setCost(item.cost);
    //    }
    //    std::ostringstream os;
    //    os << "Balance: $" << std::setprecision(4) << state.getPlayer().getBalance();
    //    scenes[SceneNames::GameField]->getChildWithName("Money Amount")->as<Label>()->changeText(os.str());
    //    if (state.getPlayer().getBalance() > Config::activePreset.winCondition) {
    //        scenes[SceneNames::GameField]->getChildWithName("Win Message")->as<MessageBox>()->hide(false);
    //        state.shutdown();
    //    }
}

void paintInventoryFrame(WorldState &state, Canvases &scenes) {
    //FIXME
    //    auto sales = scenes[SceneNames::Inventory]->getChildrenRecursively<Sale>();
    //    auto items = state.getPlayer().getSlots();
    //    for (const auto &sale: sales) {
    //        sale->setName("");
    //        sale->setItemId(-1);
    //    }
    //    for (auto[itemId, sale]: ranges::views::zip(items, sales)) {
    //        sale->setItemId(itemId);
    //        std::stringstream ss;
    //        auto item = state.getPlayer().viewItem(itemId);
    //        ss << item.fullName() << ", cost: $" << item.cost;
    //        sale->setName(ss.str());
    //    }
}
