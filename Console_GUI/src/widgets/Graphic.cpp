#include "widgets/Graphic.h"

UISize Graphic::getSize() {
    if (isHidden)
        return UISize{0, 0};
    return size;
}

void Graphic::printColumn(std::string colSymbol, int colPos, int value, int max) {
    int colSize = round((size.height - 3) * static_cast<double>(value)/max);
    for (int i = 1; i <= colSize; i++) {
        mvprintw(position.y + size.height - 1 - i,position.x + colPos, colSymbol.c_str());
    }
}

void Graphic::updateData() {
    std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
    if (end - start > delta) {
        start = std::chrono::system_clock::now();
        if (values.size() + 1 > amountOfColumns) {
            values.pop_front();
        }
        values.push_back(getNewValue());
    }
}

void Graphic::show() {
    updateData();
    int max = *std::max_element(values.begin(), values.end());

    if (isHidden) return;

    move(position.y, position.x);
    // printing OY line
    std::string symbolForPrinting;
    for (int i = 0; i < size.height; i++) {
        if (i == 0)
            symbolForPrinting = OYname;
        else if (i == 1)
            symbolForPrinting = "↑";
        else if (i == 2)
            symbolForPrinting = std::to_string(max);
        else if (i == size.height - 1)
            symbolForPrinting = "└";
        else
            symbolForPrinting = "│";
        mvprintw(position.y + i, position.x, symbolForPrinting.c_str());
    }
    // printing OX line
    for (int i = 0; i < size.width; i++) {
        if (i == 0)
            symbolForPrinting = "└";
        else if (i == size.width - 1)
            symbolForPrinting = OXname;
        else if (i == size.width - 2)
            symbolForPrinting = ">";
        else
            symbolForPrinting = "─";
        mvprintw(position.y + size.height - 1, position.x + i, symbolForPrinting.c_str());
    }

    // printing Columns
    symbolForPrinting = "$";
    if (values.empty())
        return;
    auto itr = values.begin();
    for (int i = 2; i < size.width; i+=2) {
        printColumn(symbolForPrinting, i, *itr, max);
        itr++;
        if (itr == values.end())
            break;
    }

}
