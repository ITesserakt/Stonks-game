#pragma once

#include "Widget.h"
#include "utils.h"

class PositionedWidget : public virtual Widget
{
public:
    virtual Size<unsigned> getSize() = 0;

protected:
    Size<unsigned> size;
    UIRect position;
    Align align;
};