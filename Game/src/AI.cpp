//
// Created by tesserakt on 23.09.2021.
//

#include <chrono>
#include <range/v3/all.hpp>
#include <thread>
#include <utility>

#include "AI.h"
#include "Config.h"
#include "Debug.h"

GameObject::Id AI::predictToBuy() const {
    if (world->getSlots().empty()) {
        Debug::logger << "Choosing from empty container while buying";
        return predictToBuy();
    }
    return ranges::max(world->getSlots(), {}, [this](int slot) { return world->getProfitness(slot); });
}

AI::AI(std::shared_ptr<World> state, bool debug, unsigned int maxSlots) : world(std::move(state)) {
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

std::thread AI::startTrading(bool &running) {
    using namespace std::chrono_literals;

    auto thread = std::thread([this, &running] {
        std::random_device seed;
        std::mt19937 randie(seed());
        std::uniform_int_distribution debugSleep{static_cast<int>(-Config::current().debugSpeedGame + 1), 200};
        std::uniform_int_distribution normalSleep{700, 10000};
        unsigned int sleepTime = debugFlag ? Config::current().debugSpeedGame + debugSleep(randie) : normalSleep(randie);
        while (running) {
            std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
            auto tradeType = random() % 2;
            if (!world->getSlots().empty() && couldBuy() && tradeType) {
                auto predicted = predictToBuy();
                //                auto itemShadow = world.focusItem(predicted);
                //                if (itemShadow.has_value() && getBalance() > itemShadow->cost) {
                auto item = world->takeItem(predicted);
                if (item == nullptr) continue;
                buyItem(std::move(item));
            }
            if (!getSlots().empty() && world->couldPutInto() && !tradeType) {
                std::normal_distribution multiplierDistribution{1.01, 0.1};
                auto predicted = predictToSell();
                auto predictedItem = focusItem(predicted);
                if (!predictedItem.has_value()) continue;
                auto sold = sellItem(predicted, predictedItem->cost * multiplierDistribution(randie));
                if (sold == nullptr) continue;
                world->putItem(std::move(sold));
            }
        }
    });
    return thread;
}

AI AI::fromJson(std::shared_ptr<World> world, const jsoncons::json &json) {
    auto objects = json["objects"].as<std::map<GameObject::Id, GameObject>>();
    std::map<GameObject::Id, std::unique_ptr<GameObject>> objectsPtr;
    for (const auto &[id, g] : objects)
        objectsPtr.emplace(id, std::make_unique<GameObject>(g));

    return {world,
            json["debug"].as_bool(),
            json["money"].as<double>(),
            json["availableSlots"].as_integer<unsigned int>(),
            std::move(objectsPtr),
            json["id"].as_integer<unsigned int>()};
}

AI::AI(std::shared_ptr<World> world, bool debugFlag,
       double money, unsigned int availableSlots,
       std::map<GameObject::Id, std::unique_ptr<GameObject>> container,
       unsigned int gamerId)
    : world(world), debugFlag(debugFlag), Gamer(money, availableSlots, gamerId, std::move(container)) {}
