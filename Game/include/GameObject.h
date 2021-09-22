#pragma once

#include <string>
#include <vector>
#include <ostream>
#include <sstream>
#include <numeric>
#include <algorithm>

struct GameObject {
public:
    using Name = std::string;
    using Description = std::string;
    using Cost = double;

    const Name name;
    const std::vector<Description> description;
    const Cost cost;

    [[nodiscard]] std::string fullName() const {
        std::stringstream ss;
        ss << name << " ";
        for (auto &desc: description)
            ss << desc << " ";
        return ss.str();
    }

    friend std::ostream &operator<<(std::ostream &os, const GameObject &object) {
        return os << "Object {name: " << object.name << ", desc: ["
                  << std::accumulate(++object.description.begin(), object.description.end(),
                                     *object.description.begin(), [](auto &a, auto &b) { return a + ", " + b; })
                  << "], cost: " << object.cost << "}";
    }

    bool operator==(const GameObject &rhs) const {
        return name == rhs.name &&
               description == rhs.description &&
               std::abs(cost - rhs.cost) < std::numeric_limits<GameObject::Cost>::epsilon() * 10000;
    }

    bool operator!=(const GameObject &rhs) const {
        return !(rhs == *this);
    }
};