#pragma once

#include <utility>

#include "widgets/HoverableWidget.h"
#include "GameObject.h"

class Purchase : public HoverableWidget {
private:
    std::function<void(WorldState&, Purchase&)> todo;
    GameObject::Id itemId;
    GameObject::Cost cost;

public:
    Purchase(int index, WorldState& state, std::function<void(WorldState&, Purchase&)> f);

    void click() override;

    UISize getSize() override;

    void setName(std::string newName) { name = std::move(newName); }
    void setItemId(unsigned int id) { itemId = id; }
    void setCost(GameObject::Cost newCost) { cost = newCost; }
    unsigned int getItemId() const { return itemId; }

    void show() override;
};