#pragma once

#include "Gamer.h"
#include "World.h"
#include <thread>

// This man buys products and sell them again
class AI : public Gamer {
private:
    std::shared_ptr<World> world;
    bool debugFlag;

    JSONCONS_TYPE_TRAITS_FRIEND

    AI(std::shared_ptr<World> world, bool debugFlag, double money, unsigned int availableSlots, std::map<GameObject::Id, std::unique_ptr<GameObject>> container, unsigned int gamerId);

public:
    AI(std::shared_ptr<World> state, bool debug, unsigned int maxSlots);

    static AI fromJson(std::shared_ptr<World> world, const jsoncons::json &json);

    GameObject::Id predictToBuy() const;

    GameObject::Id predictToSell() const;

    double getProfitness(GameObject::Id itemId) const;

    std::thread startTrading(bool &running);
};

namespace jsoncons {
    template <>
    struct json_type_traits<json, AI> {
        static bool is(const json &j) {
            if (!j.is_object()) return false;
            if (!j.contains("debug")) return false;
            if (!j.contains("money")) return false;
            if (!j.contains("id")) return false;
            if (!j.contains("availableSlots") && !j["availableSlots"].is_number()) return false;
            if (!j.contains("objects") && !j["objects"].is_array()) return false;
            return true;
        }

        static json to_json(const AI &bot) {
            return json{{{"debug", bot.debugFlag},
                         {"money", bot.money},
                         {"availableSlots", bot.availableSlots.load()},
                         {"id", bot.Id},
                         {"objects", static_cast<const ViewableContainer<GameObject, GameObject::Id> &>(bot)}}};
        }
    };
}// namespace jsoncons