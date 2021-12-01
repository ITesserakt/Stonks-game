#pragma once

#include <chrono>
#include <list>
#include <functional>

#include "widgets/ColorWidget.h"
#include "widgets/PositionedWidget.h"

class Graphic : public PositionedWidget, public ColorWidget {
private:
    std::string ordinance;
    std::string abscissa;
    unsigned int amountOfColumns;
    std::list<double> values;
    std::function<double()> getNewValue;

    std::chrono::duration<double> delta{1};
    std::chrono::time_point<std::chrono::system_clock> start;

    void printColumn(std::string colSymbol, int colPos, double value, double max);
    void updateData();

public:
    Graphic(const std::string &name, std::string OY, std::string OX,
            UISize sizeOfGraph, std::function<double()> getValue)
        : ordinance(std::move(OY)),
          abscissa(std::move(OX)),
          Widget(name),
          ColorWidget(name) {
        getNewValue = std::move(getValue);
        size = sizeOfGraph;
        start = std::chrono::system_clock::now();
        amountOfColumns = (sizeOfGraph.width - 3) / 2;
        init_pair(widgetId, COLOR_GREEN, COLOR_BLACK);
    }

    UISize getSize() override;

    void show() override;
};