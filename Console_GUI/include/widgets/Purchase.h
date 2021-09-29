#pragma once

#include <utility>

#include "HoverableWidget.h"

class Purchase : public HoverableWidget {
private:
    std::function<void(Purchase&)> todo;
    unsigned int itemId;
public:
    Purchase(int index, std::function<void(Purchase&)> f);

    void click() override;

    UISize getSize() override;

    void setName(std::string newName) { name = std::move(newName); }
    void setItemId(unsigned int id) { itemId = id; }
    unsigned int getItemId() { return itemId; }

    operator std::shared_ptr<Purchase>() {
        return std::make_shared<Purchase>(std::move(*this));
    }

    void show() override;
};