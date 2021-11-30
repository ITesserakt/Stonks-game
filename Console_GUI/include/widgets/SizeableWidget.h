#pragma once

#include "widgets/Widget.h"

struct SizeableWidget : virtual Widget {
    virtual UISize getSize() = 0;
};