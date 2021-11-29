//
// Created by tesserakt on 29.11.2021.
//

#include "widgets/Group.h"

UISize Group::getSize() {
    unsigned int maxx = 0;
    unsigned int maxy = 0;

    for (auto child : getChildrenWithType<PositionedWidget>()) {
        if (child->getSize().width > maxx) {
            maxx = child->getSize().width;
        }
        if (child->getSize().height > maxy) {
            maxy = child->getSize().height;
        }
    }

    return {maxx, maxy};
}

void Group::show() {
    if (isHidden) return;

    Widget::show();
}

Group::Group(std::string name) : Widget(std::move(name)) {}
