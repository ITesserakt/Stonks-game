#pragma once

#include "HoverableWidget.h"
#include <string>

class Button : public HoverableWidget
{
protected:
    bool isBlowing;
    int width;

public:
    Button(std::string name);


};