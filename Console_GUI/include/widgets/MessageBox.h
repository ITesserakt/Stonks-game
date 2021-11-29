#pragma once

#include "widgets/PositionedWidget.h"

enum SpecialPosition { Center,
                       Special };

class MessageBox : virtual public PositionedWidget {
private:
    std::string messageText;
    SpecialPosition spec;

public:
    MessageBox(std::string name, std::string text, SpecialPosition = Center);

    void show() override;

    UISize getSize() override;
};