#pragma once

#include "Gamer.h"
#include "ObjectFactory.h"
#include "ViewableContainer.h"
#include <atomic>
#include <list>
#include <vector>

class World : public ViewableContainer<GameObject, GameObject::Id> {
    friend class game_logic_profitness_Test;
    friend class WorldState;

private:
    std::vector<std::shared_ptr<Gamer>> players;
    std::atomic<unsigned int> availableSlots;
    ObjectFactory factory;

    JSONCONS_TYPE_TRAITS_FRIEND

    explicit World(unsigned int availableSlots, std::map<GameObject::Id, std::unique_ptr<GameObject>> objects);

public:
    explicit World(ObjectFactory &&factory = ObjectFactory::empty(),
                   unsigned int maxSlots = 10);

    World(World &&w) noexcept;

    [[nodiscard]] double getProfitness(const GameObject::Id &itemId) const;

    void putItem(std::unique_ptr<GameObject> obj);

    void addGamer(const std::shared_ptr<Gamer> &gamer);

    void fillUp();

    std::unique_ptr<GameObject> takeItem(GameObject::Id itemId) override;

    [[nodiscard]] bool couldPutInto() const;
};

namespace jsoncons {
    template <>
    struct json_type_traits<json, World> {
        static bool is(const json &j) {
            if (!j.is_object()) return false;
            if (!j.contains("availableSlots") && !j["availableSlots"].is_number()) return false;
            if (!j.contains("objects") && !j["objects"].is_array()) return false;
            return true;
        }

        static World as(const json &j) {
            auto objs = j["objects"].as<std::map<GameObject::Id, GameObject>>();
            std::map<GameObject::Id, std::unique_ptr<GameObject>> objsPtr;
            for (auto &[id, obj] : objs)
                objsPtr[id] = std::make_unique<GameObject>(obj);
            auto slots = j["availableSlots"].as<unsigned int>();
            return World(slots, std::move(objsPtr));
        }

        static json to_json(const World &w) {
            const ViewableContainer<GameObject, GameObject::Id> &container = w;
            return json{{{"availableSlots", w.availableSlots.load()},
                         {"objects", container}}};
        }
    };
}// namespace jsoncons
