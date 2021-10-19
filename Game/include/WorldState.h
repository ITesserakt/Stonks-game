#pragma once

#include "Player.h"
#include "AI.h"
#include "Canvas.h"

class WorldState {
private:
    std::shared_ptr<Player> player;
    std::vector<std::shared_ptr<AI>> bots;
    std::vector<std::thread> botThreads;
    World world = World(ObjectFactory::fromFile("../share/objects.json"), Config::worldSize);
    bool isActive = true;
    std::random_device engine;
    std::mt19937 random = std::mt19937(engine());

    Canvas *currentScene;
public:
    WorldState(const WorldState &) = delete;

    WorldState &operator=(const WorldState &) = delete;

    explicit WorldState(unsigned int maxBots, bool debug = false,
                        Canvas *currentScene = nullptr) :
            bots(maxBots), botThreads(maxBots), currentScene(currentScene) {
        player = std::make_shared<Player>();
        world.addGamer(player);
        for (unsigned int index = 0; index < maxBots; index++) {
            bots[index] = std::make_shared<AI>(world, debug, 10);
            world.addGamer(bots[index]);
            botThreads[index] = bots[index]->startTrading(isActive);
        }
        world.fillUp();
    }

    void changeCurrentScene(Canvas &next) {
        currentScene = &next;
    }

    Canvas &getCurrentScene() { return *currentScene; }

    void shutdown() { isActive = false; }

    World &getWorld() { return world; }

    Player &getPlayer() { return *player; }

    [[nodiscard]] bool running() const { return isActive; }

    AI &getRandomBot() { return *bots[random() % bots.size()]; }

    void joinAllAI() {
        for (auto &thread: botThreads)
            thread.join();
    }
};