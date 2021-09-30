#pragma once

#include "widgets/HoverableWidget.h"
#include "GameObject.h"

class ButtonWithObj : virtual public HoverableWidget {
protected:
    GameObject::Id objectId;
    GameObject::Cost cost;

public:
    void setItemId(unsigned int id) { objectId = id; }
    void setCost(GameObject::Cost newCost) { cost = newCost; }
    unsigned int getItemId() const { return objectId; }
};