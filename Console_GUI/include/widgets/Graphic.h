#pragma once

#include <list>
#include <chrono>
#include "widgets/PositionedWidget.h"
#include "widgets/ColorWidget.h"

class Graphic : public virtual PositionedWidget, public virtual ColorWidget {
private:
    std::string OYname;
    std::string OXname;
    int amountOfColumns;
    std::list<int> values;
    std::function<int()> getNewValue;

    std::chrono::duration<double> delta{1};
    std::chrono::time_point<std::chrono::system_clock> start;

    void printColumn(std::string colSymbol, int colPos, int value, int max);
    void updateData();
public:

    Graphic(std::string name, std::string OY, std::string OX,
            UISize sizeOfGraph, std::function<int()> getValue)
            : Widget(name),
              ColorWidget(name) {
        getNewValue = getValue;
        size = sizeOfGraph;
        OYname = OY;
        OXname = OX;
        start = std::chrono::system_clock::now();
        amountOfColumns = (sizeOfGraph.width - 3) / 2;
        init_pair(widgetId, COLOR_GREEN, COLOR_BLACK);
    }

    UISize getSize() override;

    void show() override;
};