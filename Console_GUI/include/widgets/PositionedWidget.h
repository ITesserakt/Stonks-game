#pragma once

#include "Widget.h"
#include "Utils.h"

class PositionedWidget : public virtual Widget
{
public:
    virtual Rect<unsigned> getSize() = 0;

protected:
    unsigned int height;
    unsigned int width;
    UIRect position;
    Align align;
};