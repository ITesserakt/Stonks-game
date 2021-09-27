#ifndef STONKS_GAME_PLAINTEXT_H
#define STONKS_GAME_PLAINTEXT_H

#include "PositionedWidget.h"

class PlainText: public PositionedWidget {
private:
    UISize size;

public:
    PlainText(std::string name);
    void show() override;

    UISize getSize() override { return size; }
};

#endif //STONKS_GAME_PLAINTEXT_H
