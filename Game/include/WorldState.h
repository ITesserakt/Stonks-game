#pragma once

#include <atomic>
#include <utility>

#include "AI.h"
#include "Canvas.h"
#include "Player.h"

class WorldState {
private:
    std::shared_ptr<Player> player;
    std::vector<std::shared_ptr<AI>> bots;
    std::vector<std::thread> botThreads;
    std::shared_ptr<World> world = std::make_shared<World>(ObjectFactory::fromFile("../share/objects.json"), Config::current().worldSize);
    bool isActive = true;
    std::random_device engine;
    std::mt19937 random = std::mt19937(engine());

    Canvas *currentScene = nullptr;

    explicit WorldState(std::shared_ptr<World> w, std::shared_ptr<Player> player, std::vector<std::shared_ptr<AI>> bots);

    JSONCONS_TYPE_TRAITS_FRIEND

public:
    WorldState(const WorldState &) = delete;
    WorldState &operator=(const WorldState &) = delete;
    WorldState(WorldState &&s) noexcept;
    explicit WorldState(unsigned int maxBots, bool debug = false, Canvas *currentScene = nullptr);

    void changeCurrentScene(Canvas &next);

    Canvas &getCurrentScene();

    void shutdown();

    World &getWorld();

    Player &getPlayer();

    [[nodiscard]] bool running() const;

    AI &getRandomBot();

    void cancelAllAI();
};

namespace jsoncons {
    template <>
    struct json_type_traits<json, WorldState> {
        static bool is(const json &j) {
            if (!j.is_object()) return false;
            if (!j.contains("world")) return false;
            if (!j.contains("player")) return false;
            if (!j.contains("bots")) return false;
            return true;
        }

        static WorldState as(const json &json) {
            auto world = std::make_shared<World>(json["world"].as<World>());
            auto player = std::make_shared<Player>(json["player"].as<Player>());
            std::vector<std::shared_ptr<AI>> bots;
            for (auto &&botJson : json["bots"].as<std::vector<jsoncons::json>>()) {
                auto bot = std::make_shared<AI>(AI::fromJson(world, botJson));
                world->addGamer(bot);
                bots.push_back(bot);
            }

            world->addGamer(player);
            return WorldState(world, player, bots);
        }

        static json to_json(const WorldState &w) {
            std::vector<AI> bots;
            for (const auto &bot : w.bots)
                bots.emplace_back(std::move(*bot));

            return json{{{"world", *w.world},
                         {"player", *w.player},
                         {"bots", bots}}};
        }
    };
}// namespace jsoncons