#pragma once

#include <string>
#include <utility>
#include <vector>
#include <ostream>
#include <sstream>
#include <limits>
#include <algorithm>
#include <numeric>
#include <memory>

class Gamer;

struct GameObject {
public:
    using Name = std::string;
    using Description = std::string;
    using Cost = double;
    using Id = unsigned int;

    const Name name;
    const std::vector<Description> description;
    const Id id;
    Cost cost;
    unsigned int timesSold = 0;
    std::shared_ptr<Gamer> lastSeller = {};

    [[nodiscard]] std::string fullName() const;

    friend std::ostream &operator<<(std::ostream &os, const GameObject &object);

    bool operator==(const GameObject &rhs) const;

    bool operator!=(const GameObject &rhs) const;

    GameObject(Name name, std::vector<Description> descs, Id id, Cost cost) :
            name(std::move(name)), description(std::move(descs)), id(id), cost(cost) {}
};