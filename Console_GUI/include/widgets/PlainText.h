#ifndef STONKS_GAME_PLAINTEXT_H
#define STONKS_GAME_PLAINTEXT_H

#include "PositionedWidget.h"
#include "ColorWidget.h"

class PlainText: public virtual PositionedWidget, public virtual ColorWidget {
private:
    UISize size;

public:
    explicit PlainText(std::string name);
    void show() override;

    UISize getSize() override { return size; }
};

#endif //STONKS_GAME_PLAINTEXT_H
