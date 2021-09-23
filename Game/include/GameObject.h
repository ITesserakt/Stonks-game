#pragma once

#include <string>
#include <vector>
#include <ostream>
#include <sstream>
#include <limits>
#include <algorithm>
#include <numeric>

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
    unsigned int timesSold;

    [[nodiscard]] std::string fullName() const;

    friend std::ostream &operator<<(std::ostream &os, const GameObject &object);

    bool operator==(const GameObject &rhs) const;

    bool operator!=(const GameObject &rhs) const;
};