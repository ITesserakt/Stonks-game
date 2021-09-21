#pragma once

#include "PositionedWidget.h"
#include "../EventSequence.h"

class HoverableWidget : public virtual PositionedWidget
{
public:
    virtual EventSequence onHoverStart() = 0;
    virtual EventSequence onHoverEnd() = 0;
    virtual EventSequence click() = 0;
};