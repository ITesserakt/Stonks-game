#pragma once

#include "ButtonWithObj.h"
#include "Canvas.h"

class Sale : public ButtonWithObj {
private:
    int newPrice = 100;
public:
    Sale(int index, WorldState& state, std::function<void(WorldState &,
                                                          Sale &)> f);
    int getPrice() { return newPrice; }
    void appendNewPrice(int append) {
        if (newPrice + append >= 0)
            newPrice += append;
    }
    void updatePrice() { newPrice = 100; }
    UISize getSize() override;
    void show() override;
};