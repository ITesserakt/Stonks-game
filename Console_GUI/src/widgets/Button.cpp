#include "widgets/Button.h"
#include "Helper.h"

Button::Button(std::string name) {
    isBlowing = false;
    width = getHeight(name);
}
