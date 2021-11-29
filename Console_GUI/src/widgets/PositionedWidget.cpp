//
// Created by tesserakt on 29.11.2021.
//

#include "widgets/PositionedWidget.h"

void PositionedWidget::changePos(unsigned int x, unsigned int y) {
    position.x = x;
    position.y = y;
}

void PositionedWidget::hide(bool hide) {
    this->isHidden = hide;
    for (auto child : getChildrenWithType<PositionedWidget>()) {
        child->hide(isHidden);
    }
}

bool PositionedWidget::isActive() const { return !isHidden; }
