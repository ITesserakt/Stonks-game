#pragma once

#include "widgets/HoverableWidget.h"
#include "GameObject.h"
#include "optional.hpp"

class TradeButton : public virtual HoverableWidget {
protected:
    std::experimental::optional<GameObject> object;

public:
    void clearItem();
    bool isObjectExist();
    GameObject::Id getItemId();
};