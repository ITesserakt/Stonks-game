#include "Canvas.h"
#include "widgets/SizeableWidget.h"

void Canvas::show() {
    int y = 0;
    int x = 0;

    if (align == Left) {
        for (auto child : getChildrenWithType<PositionedWidget>()) {
            child->changePos(x, y);// DON'T USE move() from ncurses
            child->show();
            y += child->getSize().height;
        }
    }
    else if (align == Centered) {
        int xMax = this->getSize().width;
        int yMax = this->getSize().height;

        int w = 0;
        int h = 0;
        for (const auto &child : this->getChildrenRecursively<SizeableWidget>()) {
            if (child->getSize().width > w) { w = child->getSize().width; }
            if (child->getSize().height > h) { h = child->getSize().height; }
        }

        int xInd = (xMax - w) / 2;// x Indent
        int yInd = (yMax - h) / 2;

        for (auto child : getChildrenWithType<PositionedWidget>()) {
            child->changePos(xInd, yInd);
            child->show();
            yInd += child->getSize().height;
        }
    }
    else if (align == Right) {
        x = getSize().width;
        for (auto child : getChildrenWithType<PositionedWidget>()) {
            child->changePos(x - child->getSize().width, y);
            child->show();
            y += child->getSize().height;
        }
    }
    else if (align == Vsplit) {
        int xMax          = this->getSize().width;
        int yMax          = this->getSize().height;
        int windowsAmount = children.size();
        int xInd          = xMax / windowsAmount;// x Indent

        for (auto child : getChildrenWithType<PositionedWidget>()) {
            child->changePos(x, y);
            child->show();
            x += xInd + 1;
        }

        // printing center line
        for (int j = 1; j <= windowsAmount; j++) {
            for (int i = 0; i < yMax; i++) mvprintw(i, xInd, "│");
            xInd *= j;
        }
    }
}

std::shared_ptr<HoverableWidget> findMinAbove(const std::vector<std::shared_ptr<HoverableWidget>> &from,
        unsigned int                                                                               above) {
    int                              minIndex = INT32_MAX;
    std::shared_ptr<HoverableWidget> associated;
    for (const auto &x : from)
        if (x->getTabIndex() >= above && x->getTabIndex() < minIndex && x->isActive()) {
            minIndex   = x->getTabIndex();
            associated = x;
        }
    if (minIndex == INT32_MAX) return nullptr;
    return associated;
}

std::shared_ptr<HoverableWidget> findMaxBelow(const std::vector<std::shared_ptr<HoverableWidget>> &from,
        unsigned int                                                                               below) {
    int                              maxIndex = INT32_MIN;
    std::shared_ptr<HoverableWidget> associated;
    for (const auto &x : from)
        if (x->getTabIndex() <= below && x->getTabIndex() > maxIndex && x->isActive()) {
            maxIndex   = x->getTabIndex();
            associated = x;
        }
    if (maxIndex == INT32_MIN) return nullptr;
    return associated;
}

void Canvas::changeActiveWidget(Direction direct, unsigned int length) {
    if (activeWidget == nullptr) return;
    std::shared_ptr<HoverableWidget> next;
    auto                             toGo = getChildrenRecursively<HoverableWidget>();
    switch (direct) {
        case UP:
            next = findMaxBelow(toGo, activeWidget->getTabIndex() == 0 ? 0 : activeWidget->getTabIndex() - length);
            break;
        case DOWN: next = findMinAbove(toGo, activeWidget->getTabIndex() + length); break;
    }
    if (next != nullptr) {
        activeWidget->onHoverEnd();
        next->onHoverStart();
        activeWidget = next;
    }
}

void Canvas::bind(std::shared_ptr<Widget> widget) {
    BindableWidget::bind(widget);
    if (widget->is<HoverableWidget>() && widget->as<HoverableWidget>()->getTabIndex() == 0)
        activeWidget = widget->as<HoverableWidget>();
    else if (widget->is<BindableWidget>())
        for (auto x : widget->as<BindableWidget>()->getChildrenRecursively<HoverableWidget>())
            if (x->getTabIndex() == 0) activeWidget = x;
    if (activeWidget != nullptr) activeWidget->onHoverStart();
}

UISize Canvas::getSize() {
    return {(unsigned) getmaxx(stdscr), (unsigned) getmaxy(stdscr)};
}
