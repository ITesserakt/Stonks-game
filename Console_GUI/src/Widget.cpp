//
// Created by Сева on 10.09.2021.
//

#include "Widget.h"
#include "Helper.h"

Button::Button(std::string name) {
    isBlowing = false;
    width     = getHeight(name);
}
