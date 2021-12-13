#include "game_widgets/TradeButton.h"

void TradeButton::clearItem() {
    object.reset();
    setName("");
}

bool TradeButton::isObjectExist() { return object.has_value(); }

GameObject::Id TradeButton::getItemId() { return object->id; }
