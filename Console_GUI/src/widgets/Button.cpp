#include "widgets/Button.h"
#include <utility>

Button::Button(std::string name) {
    isBlowing = false;
    height = getHeight(std::move(name));
}
