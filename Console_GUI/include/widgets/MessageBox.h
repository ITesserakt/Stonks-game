#pragma once

#include "widgets/PositionedWidget.h"

enum specialPosition {Center, Special};

class MessageBox : virtual public PositionedWidget {
private:
    std::string messageText;
    specialPosition spec;
public:
    MessageBox(std::string name, std::string text, specialPosition = Center);

    void show() override;

    UISize getSize() override;
};