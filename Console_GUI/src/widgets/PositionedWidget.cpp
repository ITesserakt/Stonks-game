//
// Created by tesserakt on 29.11.2021.
//

#include "widgets/PositionedWidget.h"


bool PositionedWidget::isActive() const { return !isHidden; }

void PositionedWidget::changePos(unsigned int x, unsigned int y) {
    position.x = x;
    position.y = y;
    for (auto& child : this->getChildrenRecursively<PositionedWidget>()) {
        child->changePos(x + child->getSize().width, y + child->getSize().height);
    }
}

void PositionedWidget::hide(bool hide) {
    this->isHidden = hide;
    for (auto child: getChildrenWithType<PositionedWidget>()) {
        child->hide(isHidden);
    }
}
