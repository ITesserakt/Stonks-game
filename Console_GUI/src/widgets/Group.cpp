//
// Created by tesserakt on 29.11.2021.
//

#include "widgets/Group.h"


Group::Group(const std::string &name) : Widget(name) {}

UISize Group::getSize() {
    unsigned int maxx = 0;
    unsigned int maxy = 0;

    for (auto child : getChildrenWithType<SizeableWidget>()) {
        if (child->getSize().width > maxx)
            maxx = child->getSize().width;
        maxy += child->getSize().height;
    }

    return {maxx, maxy};
}

void Group::show() {
    if (isHidden) return;
    int x = position.x;
    int y = position.y;
    for (auto child : getChildrenWithType<PositionedWidget>()) {
        child->changePos(x, y);// DON'T USE move() from ncurses
        child->show();
        y += child->getSize().height;
    }
}
