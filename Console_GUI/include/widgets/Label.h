#pragma once

#include <utility>

#include "PositionedWidget.h"
#include "ColorWidget.h"

class Label: public virtual PositionedWidget, public virtual ColorWidget {
private:
    std::string text;

public:
    Label(std::string name, const std::string& text);
    void show() override;
    void updateText(std::string next) { text = std::move(next); }

    UISize getSize() override { return size; }
};