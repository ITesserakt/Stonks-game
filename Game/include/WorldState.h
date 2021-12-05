#pragma once

#include <utility>

#include "AI.h"
#include "Canvas.h"
#include "Player.h"

class WorldState {
private:
    std::shared_ptr<Player> player;
    std::vector<std::shared_ptr<AI>> bots;
    std::vector<std::thread> botThreads;
    World world{ObjectFactory::fromFile("../share/objects.json"), Config::current().worldSize};
    bool isActive = true;
    std::random_device engine;
    std::mt19937 random = std::mt19937(engine());

    Canvas *currentScene = nullptr;

    explicit WorldState(World &&w, std::shared_ptr<Player> &&player, std::vector<std::shared_ptr<AI>> &&bots);

public:
    WorldState(const WorldState &) = delete;
    WorldState &operator=(const WorldState &) = delete;
    WorldState(WorldState &&s) noexcept;
    explicit WorldState(unsigned int maxBots, bool debug = false, Canvas *currentScene = nullptr);

    static WorldState fromJson(const jsoncons::json &json);

    void writeToJson(jsoncons::json &json);

    void changeCurrentScene(Canvas &next);

    Canvas &getCurrentScene();

    void shutdown();

    World &getWorld();

    Player &getPlayer();

    [[nodiscard]] bool running() const;

    AI &getRandomBot();

    void cancelAllAI();
};