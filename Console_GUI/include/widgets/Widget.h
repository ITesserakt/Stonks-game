#pragma once

#pragma once

#include "utils.h"
#include <vector>
#include <string>
#include <memory>

class Widget
{
    // TODO: Base class to represent widget, i.e. text label, button, etc
protected:
    std::string name;
    // std::weak_ptr<Widget> parent;
    // std::shared_ptr<Widget> canvas;
    // std::vector<std::shared_ptr<Widget>> children;

public:
    virtual void show() = 0;
    virtual void abstract() = 0;
};