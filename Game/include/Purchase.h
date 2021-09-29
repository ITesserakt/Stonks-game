#pragma once

#include <utility>

#include "widgets/HoverableWidget.h"
#include "GameObject.h"

class Purchase : public HoverableWidget {
private:
    GameObject::Id itemId;
    GameObject::Cost cost;

public:
    Purchase(int index, std::function<void(HoverableWidget&)> f);

    UISize getSize() override;

    void setItemId(unsigned int id) { itemId = id; }
    void setCost(GameObject::Cost newCost) { cost = newCost; }
    unsigned int getItemId() { return itemId; }

    void show() override;
};