#pragma once

#include "ButtonWithObj.h"
#include "Canvas.h"

class Sale : public ButtonWithObj {
private:
    double newPrice = 100;
public:
    Sale(int index, WorldState& state, std::function<void(WorldState &,
                                                          Sale &)> f);
    double getPrice() const { return newPrice; }
    void appendNewPrice(double append) {
        if (newPrice + append >= 0)
            newPrice += append;
    }
    void updatePrice(double value) { newPrice = value; }
    UISize getSize() override;
    void show() override;
};