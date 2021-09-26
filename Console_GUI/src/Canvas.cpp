#include "Canvas.h"
#include "widgets/Group.h"
#include <ncurses.h>

void Canvas::show() {
    int y = 0;
    int x = 0;

    if (isHidden) { return; }

    if (align == Left) {
        for (const auto &child: children) {
            child->changePos(x, y);             // DON'T USE move() from ncurses
            child->show();
            y += child->getSize().height;
        }
    }
    else if (align == Centered) {
        auto gr = std::make_shared<Group>("summering");
        int xMax = this->getSize().width;
        int yMax = this->getSize().height;

        for (const auto &child: children) { gr->bind(child); }
        int w = gr->getSize().width;
        int h = gr->getSize().height;

        int xInd = (xMax - w) / 2; // x Indent
        int yInd = (yMax - h) / 2;

        for (const auto& child: children) {
            child->changePos(xInd, yInd);
            child->show();
            yInd += child->getSize().height;
        }
    }
    else if (align == Right) {
        x = getmaxx(initscr());
        for (const auto &child: children) {
            child->changePos(x - child->getSize().width, y);
            child->show();
            y += child->getSize().height;
        }
    }
}

UISize Canvas::getSize() {
    UISize sizeOfWindow;
    sizeOfWindow.width  = getmaxx(initscr());
    sizeOfWindow.height = getmaxy(initscr());
    return sizeOfWindow;
}

std::shared_ptr<HoverableWidget> Canvas::whoOnHover() {
    for (const auto& child: children) {
        if (child->isClickable()) { return std::dynamic_pointer_cast<HoverableWidget>(child); }
    }
    return nullptr;
}

void Canvas::changeActiveWidget(Direction direct) {

}
