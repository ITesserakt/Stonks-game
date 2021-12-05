#pragma once

#include <jsoncons/json.hpp>
#include <limits>
#include <memory>
#include <numeric>
#include <ostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

class Gamer;

struct GameObject {
public:
    using Name = std::string;
    using Description = std::string;
    using Cost = double;
    using Id = unsigned int;

private:
    JSONCONS_TYPE_TRAITS_FRIEND

    GameObject(Name name, std::vector<Description> desc, const Id id, Cost cost, unsigned int timesSold);

public:
    const Name name;
    const std::vector<Description> description;
    const Id id;
    Cost cost;
    unsigned int timesSold = 0;
    std::shared_ptr<Gamer> lastSeller = {};

    // This function returns name and all description of items
    [[nodiscard]] std::string fullName() const;

    friend std::ostream &operator<<(std::ostream &os, const GameObject &object);

    bool operator==(const GameObject &rhs) const;

    bool operator!=(const GameObject &rhs) const;

    GameObject(Name name, std::vector<Description> descs, Id id, Cost cost) : name(std::move(name)), description(std::move(descs)), id(id), cost(cost) {}
};

namespace jsoncons {
    template <>
    struct json_type_traits<json, GameObject> {
        static bool is(const json &j) {
            if (!j.is_object()) return false;
            if (!j.contains("name")) return false;
            if (!j.contains("description")) return false;
            if (!j.contains("id")) return false;
            if (!j.contains("cost")) return false;
            if (!j.contains("timesSold")) return false;
            return true;
        }

        static GameObject as(const json &j) {
            auto name = j["name"].as<GameObject::Name>();
            auto desc = j["description"].as<std::vector<GameObject::Description>>();
            auto id = j["id"].as<GameObject::Id>();
            auto cost = j["cost"].as<GameObject::Cost>();
            auto timesSold = j["timesSold"].as<unsigned int>();

            return GameObject{name, desc, id, cost, timesSold};
        }

        static json to_json(const GameObject &g) {
            return json{{{"name", g.name},
                         {"description", g.description},
                         {"id", g.id},
                         {"cost", g.cost},
                         {"timesSold", g.timesSold}}};
        }
    };
}// namespace jsoncons
