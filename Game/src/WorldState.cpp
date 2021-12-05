//
// Created by tesserakt on 05.12.2021.
//

#include "WorldState.h"

WorldState::WorldState(World &&w, std::shared_ptr<Player> &&player, std::vector<std::shared_ptr<AI>> &&bots)
    : world(std::move(w)), player(std::move(player)), bots(std::move(bots)) {
    botThreads.resize(bots.size());
    for (unsigned int index = 0; index < bots.size(); index++)
        botThreads[index] = bots[index]->startTrading(isActive);
}

WorldState::WorldState(WorldState &&s) noexcept
    : player(std::move(s.player)),
      bots(std::move(s.bots)),
      botThreads(std::move(s.botThreads)),
      world(std::move(s.world)),
      isActive(s.isActive) {}

WorldState WorldState::fromJson(const jsoncons::json &json) {
    auto world = json["world"].as<World>();
    auto player = std::make_shared<Player>(json["player"].as<Player>());
    std::vector<std::shared_ptr<AI>> bots;
    for (auto &&botJson : json["bots"].as<std::vector<jsoncons::json>>()) {
        bots.push_back(std::make_shared<AI>(AI::fromJson(world, botJson)));
    }

    ranges::for_each(bots, [&world](auto ai) { world.addGamer(ai); });
    world.addGamer(player);
    return WorldState(std::move(world), std::move(player), std::move(bots));
}

WorldState::WorldState(unsigned int maxBots, bool debug, Canvas *currentScene)
    : bots(maxBots),
      botThreads(maxBots),
      currentScene(currentScene) {
    player = std::make_shared<Player>();
    world.addGamer(player);
    for (unsigned int index = 0; index < maxBots; index++) {
        bots[index] = std::make_shared<AI>(world, debug, 10);
        world.addGamer(bots[index]);
        botThreads[index] = bots[index]->startTrading(isActive);
    }
    world.fillUp();
}

void WorldState::changeCurrentScene(Canvas &next) {
    currentScene = &next;
}

Canvas &WorldState::getCurrentScene() { return *currentScene; }

void WorldState::shutdown() { isActive = false; }

World &WorldState::getWorld() { return world; }

Player &WorldState::getPlayer() { return *player; }

bool WorldState::running() const { return isActive; }

AI &WorldState::getRandomBot() { return *bots[random() % bots.size()]; }

void WorldState::cancelAllAI() {
    for (auto &thread : botThreads) {
        thread.detach();
        thread.std::thread::~thread();
    }
}

void WorldState::writeToJson(jsoncons::json &json) {
    json["world"] = world;
    json["player"] = *player;
    std::vector<jsoncons::json> botsJson;
    for (const auto &bot : bots) {
        jsoncons::json j;
        bot->writeToJson(j);
        botsJson.emplace_back(j);
    }
    json["bots"] = botsJson;
}
