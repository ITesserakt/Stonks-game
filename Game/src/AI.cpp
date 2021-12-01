//
// Created by tesserakt on 23.09.2021.
//

#include <chrono>
#include <range/v3/all.hpp>
#include <thread>

#include "AI.h"
#include "Config.h"
#include "Debug.h"

GameObject::Id AI::predictToBuy() const {
    if (world.getSlots().empty()) {
        Debug::logger << "Choosing from empty container while buying";
        return predictToBuy();
    }
    return ranges::max(world.getSlots(), {}, [this](int slot) { return world.getProfitness(slot); });
}

AI::AI(World &state, bool debug, unsigned int maxSlots) : world(state) {
    money = 10000;
    availableSlots = maxSlots;
    debugFlag = debug;
}

GameObject::Id AI::predictToSell() const {
    if (getSlots().empty()) {
        Debug::logger << "Choosing from empty container while selling";
        return predictToSell();
    }
    return ranges::max(getSlots(), {}, [this](GameObject::Id id) { return getProfitness(id); });
}

double AI::getProfitness(GameObject::Id itemId) const {
    auto item = focusItem(itemId);
    if (!item.has_value()) return std::numeric_limits<double>::min();

    unsigned int sellByKind = 0;
    for (const auto &pair : view())
        if (pair.second.name == item->name)
            sellByKind++;
    return (availableSlots + 0.1) / sellByKind / (item->timesSold + 1) * item->cost;
}

std::thread AI::startTrading(const bool &running) {
    auto thread = std::thread([this, &running] {
        std::random_device seed;
        std::mt19937 randie(seed());
        unsigned int sleepTime = debugFlag ? Config::debugSpeedGame + randie() % 90 : 700 + randie() % 10000;
        while (running) {
            std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
            auto tradeType = random() % 2;
            if (!world.getSlots().empty() && couldBuy() && tradeType) {
                auto predicted = predictToBuy();
                //                if (getBalance() > world.viewItem(predicted).cost)
                auto item = world.takeItem(predicted);
                if (item == nullptr) continue;
                buyItem(std::move(item));
            }
            if (!getSlots().empty() && world.couldPutInto() && !tradeType) {
                auto predicted = predictToSell();
                auto costMult = (1 + (double) (-10 + (int) randie() % 40) / 100);
                auto predictedItem = focusItem(predicted);
                if (!predictedItem.has_value()) continue;
                auto sold = sellItem(predicted, predictedItem->cost * costMult);
                if (sold == nullptr) continue;
                world.putItem(std::move(sold));
            }
        }
    });
    thread.detach();
    return thread;
}
