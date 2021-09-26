//
// Created by tesserakt on 23.09.2021.
//

#include "GameObject.h"
#include "utils.h"

std::ostream &operator<<(std::ostream &os, const GameObject &object) {
    return os << "Object {name: " << object.name << ", desc: ["
              << joinToString(object.description.begin(),  object.description.end(), ", ")
              << "], cost: " << object.cost << ", times sold: " << object.timesSold << "}";
}

bool GameObject::operator==(const GameObject &rhs) const {
    return name == rhs.name &&
           description == rhs.description &&
           std::abs(cost - rhs.cost) < std::numeric_limits<GameObject::Cost>::epsilon() * 10000 &&
           timesSold == rhs.timesSold &&
           id == rhs.id;
}

bool GameObject::operator!=(const GameObject &rhs) const {
    return !(rhs == *this);
}

std::string GameObject::fullName() const {
    std::stringstream ss;
    ss << name << " ";
    for (auto &desc: description)
        ss << desc << " ";
    return ss.str();
}
