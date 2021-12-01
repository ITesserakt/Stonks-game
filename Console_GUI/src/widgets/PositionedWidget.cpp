//
// Created by tesserakt on 29.11.2021.
//

#include "widgets/PositionedWidget.h"
#include "widgets/BindableWidget.h"

bool PositionedWidget::isActive() const { return !isHidden; }

void PositionedWidget::changePos(unsigned int x, unsigned int y) {
    position.x = x;
    position.y = y;
    if (is<BindableWidget>())
        for (auto &child : as<BindableWidget>()->getChildrenRecursively<PositionedWidget>())
            child->changePos(x + child->getSize().width, y + child->getSize().height);
}

void PositionedWidget::hide(bool hide) {
    this->isHidden = hide;
    if (is<BindableWidget>())
        for (auto child : as<BindableWidget>()->getChildrenWithType<PositionedWidget>())
            child->hide(isHidden);
}
