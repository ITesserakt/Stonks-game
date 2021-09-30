#pragma once

#include "Player.h"
#include "AI.h"
#include "Canvas.h"

class WorldState {
private:
    std::shared_ptr<Player> player;
    std::vector<std::shared_ptr<AI>> bots;
    World world = World(ObjectFactory::fromFile("../share/objects.json"));
    bool isActive = true;
    std::random_device engine;
    std::mt19937 random = std::mt19937(engine());

    Canvas* currentScene;
public:
    WorldState(const WorldState &) = delete;

    WorldState &operator=(const WorldState &) = delete;

    explicit WorldState(Canvas& currentScene, unsigned int maxBots):
            bots(maxBots), currentScene(&currentScene) {
        player = std::make_shared<Player>();
        world.addGamer(player);
        for (unsigned int index = 0; index < maxBots; index++) {
            bots[index] = std::make_shared<AI>(world);
            world.addGamer(bots[index]);
        }
        world.fillUp();
    }

    void changeCurrentScene(Canvas& next) {
        currentScene = &next;
    }

    Canvas& getCurrentScene() { return *currentScene; }

    void shutdown() { isActive = false; }

    World& getWorld() { return world; }

    Player& getPlayer() { return *player; }

    bool running() { return isActive; }

    AI& getRandomBot() { return *bots[random() % bots.size()]; }
};