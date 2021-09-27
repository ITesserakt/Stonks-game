#pragma once

#include "widgets/Widget.h"

class SizeableWidget: public virtual Widget {
public:
    virtual UISize getSize() = 0;
};