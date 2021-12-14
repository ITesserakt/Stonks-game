//
// Created by tesserakt on 05.12.2021.
//

#include "WorldState.h"

WorldState::WorldState(std::shared_ptr<World> w, std::shared_ptr<Player> player, std::vector<std::shared_ptr<AI>> bots)
    : world(std::move(w)), player(std::move(player)), bots(std::move(bots)) {
    botThreads.resize(this->bots.size());
}

WorldState::WorldState(WorldState &&s) noexcept
    : player(std::move(s.player)),
      bots(std::move(s.bots)),
      world(std::move(s.world)) {
    botThreads.resize(this->bots.size());
}

WorldState::WorldState(unsigned int maxBots, bool debug, Canvas *currentScene)
    : bots(maxBots),
      botThreads(maxBots),
      currentScene(currentScene) {
    player = std::make_shared<Player>();
    world->addGamer(player);
    for (unsigned int index = 0; index < maxBots; index++) {
        bots[index] = std::make_shared<AI>(world, debug, 10);
        world->addGamer(bots[index]);
    }
    world->fillUp();
}

void WorldState::changeCurrentScene(Canvas &next) {
    currentScene = &next;
}

Canvas &WorldState::getCurrentScene() { return *currentScene; }

void WorldState::shutdown() { isActive = false; }

World &WorldState::getWorld() { return *world; }

Player &WorldState::getPlayer() { return *player; }

bool WorldState::running() const {
    return isActive;
}

AI &WorldState::getRandomBot() {
    return *bots[random() % bots.size()];
}

void WorldState::cancelAllAI() {
    for (auto &thread : botThreads) {
        thread.detach();
        thread.std::thread::~thread();
    }
}

void WorldState::run() {
    if (isPaused) {
        isPaused = false;
        for (unsigned int index = 0; index < this->bots.size(); index++)
            botThreads[index] = this->bots[index]->startTrading(isActive);
    }
}
