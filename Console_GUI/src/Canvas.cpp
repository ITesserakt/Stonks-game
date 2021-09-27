#include "Canvas.h"
#include "widgets/Group.h"

void Canvas::show() {
    int y = 0;
    int x = 0;

    if (align == Left) {
        for (auto child: getChildrenWithType<PositionedWidget>()) {
            child->changePos(x, y);             // DON'T USE move() from ncurses
            child->show();
            y += child->getSize().height;
        }
    } else if (align == Centered) {
        auto gr = std::make_shared<Group>("summering");
        int xMax = this->getSize().width;
        int yMax = this->getSize().height;

        for (const auto &child: children) { gr->bind(child); }
        int w = gr->getSize().width;
        int h = gr->getSize().height;

        int xInd = (xMax - w) / 2; // x Indent
        int yInd = (yMax - h) / 2;

        for (auto child: getChildrenWithType<PositionedWidget>()) {
            child->changePos(xInd, yInd);
            child->show();
            yInd += child->getSize().height;
        }
    } else if (align == Right) {
        x = getSize().width;
        for (auto child: getChildrenWithType<PositionedWidget>()) {
            child->changePos(x - child->getSize().width, y);
            child->show();
            y += child->getSize().height;
        }
    }
}

void Canvas::changeActiveWidget(Direction direct) {
    if (activeWidget == nullptr)
        return;
    activeWidget->onHoverEnd();
    for (auto x: getChildrenRecursively<HoverableWidget>())
        if (x->getTabIndex() == activeWidget->getTabIndex() + direct) {
            activeWidget = x;
            break;
        }
    activeWidget->onHoverStart();
}

void Canvas::firstOnHover() {
    if (activeWidget != nullptr)
        activeWidget->onHoverStart();
}

void Canvas::bind(std::shared_ptr<Widget> widget) {
    canvas = shared_from_this()->as<Canvas>();
    Widget::bind(widget);
    for (auto x: getChildrenRecursively<HoverableWidget>())
        if (x->getTabIndex() == 0) {
            activeWidget = x;
            activeWidget->onHoverStart();
        }
}

UISize Canvas::getSize() {
    return {(unsigned) getmaxx(stdscr), (unsigned) getmaxy(stdscr)};
}
