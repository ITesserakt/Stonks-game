#include "widgets/MessageBox.h"

#include <utility>
#include "utils.h"
#include "sstream"

void paintLine(std::stringstream& ss, const std::string& text) {
    ss << '+';
    for (int i = 0; i < text.size(); i++) {
        ss << '-';
    }
    ss << '+';
}
void paintInner(std::stringstream& ss, const std::string& text) {
    ss << '|';
    ss << text;
    ss << '|';
}

MessageBox::MessageBox(std::string name, std::string text, specialPosition spec)
: Widget(std::move(name)), spec(spec) {
    std::stringstream ss;
    paintLine(ss, text); ss << '\n';
    paintInner(ss, text); ss << '\n';
    paintLine(ss, text);
    messageText = ss.str();
}

void MessageBox::show() {
    if (isHidden) return;
    if (spec == specialPosition::Center) {
        int xMax = getmaxx(stdscr);
        int yMax = getmaxy(stdscr);
        for (int i = 0; i < getHeight(messageText); i++) {
            move((yMax - size.height) / 2 + i, (xMax - size.width) / 2);
            printw("%s", splitBySentence(messageText, i).c_str());
        }
    }
    else if (spec == specialPosition::Special){
        move(position.y, position.x);
        //printw("%s", messageText.c_str());
    }
}

UISize MessageBox::getSize() {
    if (isHidden) size = {0, 0};
    else size = {getWidth(messageText), getHeight(messageText)};
    return size;
}
