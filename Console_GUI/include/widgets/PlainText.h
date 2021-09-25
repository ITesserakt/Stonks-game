#ifndef STONKS_GAME_PLAINTEXT_H
#define STONKS_GAME_PLAINTEXT_H

#include "PositionedWidget.h"

class PlainText: public PositionedWidget {
public:
    PlainText(std::string name);
    void show() override;

    Size<unsigned int> getSize() override { return size; }
};

#endif //STONKS_GAME_PLAINTEXT_H
