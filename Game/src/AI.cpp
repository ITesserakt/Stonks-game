//
// Created by tesserakt on 23.09.2021.
//

#include <AI.h>
#include "Config.h"
#include <range/v3/all.hpp>
#include <thread>
#include <chrono>

GameObject::Id AI::predictToBuy() {
    return ranges::max(world.getSlots(), {}, [&](int slot) { return world.getProfitness(slot); });
}

AI::AI(World &state, bool debug, unsigned int maxSlots) : world(state) {
    money = 10000;
    availableSlots = maxSlots;
    debugFlag = debug;
}

GameObject::Id AI::predictToSell() {
    return ranges::max(getSlots(), {}, [&](int slot) { return getProfitness(slot); });
}

double AI::getProfitness(GameObject::Id itemId) {
    auto item = viewItem(itemId);
    auto sellByKind = ranges::count_if(container, [&](const auto &a) {
        return a.second->name == item.name;
    });
    return (availableSlots + 0.1) / sellByKind / (item.timesSold + 1) * item.cost;
}

std::thread AI::startTrading(const bool& running) {
    return std::thread([&] {
        std::random_device seed;
        std::mt19937 randie(seed());
        unsigned int sleepTime;
        if (debugFlag) { sleepTime = Config::debugSpeedGame + randie() % 90; }
        else { sleepTime = 700 + randie() % 10000; }
        while (running) {
            std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
            auto tradeType = random() % 2;
            if (world.getSlots().size() != 0 && couldBuy() && tradeType) {
                auto predicted = predictToBuy();
//                if (getBalance() > world.viewItem(predicted).cost)
                    buyItem(world.takeItem(predicted));
            }
            if (getSlots().size() != 0 && world.couldPutInto() && !tradeType) {
                auto predicted = predictToSell();
                auto costMult = (1 + (double) (-10 + (int)randie() % 40) / 100);
                auto newCost = viewItem(predicted).cost * costMult;
                world.putItem(sellItem(predicted, newCost));
            }
        }
    });
}
