#pragma once

#include "SizeableWidget.h"

class PositionedWidget : public virtual SizeableWidget {
public:
    void changePos(unsigned int x, unsigned int y);

    virtual void hide(bool hide = true);

    bool isActive() const;

protected:
    bool isHidden = false;
    UIPoint position;
    Align align;
    UISize size;
};