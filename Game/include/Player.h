#pragma once

#include "Config.h"
#include "Gamer.h"

class Player : public Gamer {
private:
    unsigned int inventorySize;

    JSONCONS_TYPE_TRAITS_FRIEND

    Player(unsigned int inventorySize,
           double money,
           unsigned int availableSlots,
           std::map<GameObject::Id, std::unique_ptr<GameObject>> container,
           unsigned int playerId);

public:
    explicit Player();
    unsigned int getInventorySize() const;
};

namespace jsoncons {
    template <>
    struct json_type_traits<json, Player> {
        static bool is(const json &j) {
            if (!j.is_object()) return false;
            if (!j.contains("inventorySize")) return false;
            if (!j.contains("money")) return false;
            if (!j.contains("availableSlots")) return false;
            if (!j.contains("objects")) return false;
            if (!j.contains("Id")) return false;
            return true;
        }

        static Player as(const json &j) {
            auto invSize = j["inventorySize"].as<unsigned int>();
            auto money = j["money"].as<double>();
            auto availableSlots = j["availableSlots"].as<unsigned int>();
            auto objs = j["objects"].as<std::map<GameObject::Id, GameObject>>();
            auto PlayerId = j["Id"].as<unsigned int>();
            std::map<GameObject::Id, std::unique_ptr<GameObject>> objsPtr;
            for (auto &[id, obj] : objs)
                objsPtr[id] = std::make_unique<GameObject>(obj);

            return {invSize, money, availableSlots, std::move(objsPtr), PlayerId};
        }

        static json to_json(const Player &g) {
            return json{{{"inventorySize", g.inventorySize},
                         {"money", g.money},
                         {"availableSlots", g.availableSlots.load()},
                         {"objects", g.container},
                         {"id", g.Id}}};
        }
    };
}// namespace jsoncons
