#pragma once

#include <utility>
#include <functional>
#include <chrono>
#include "PositionedWidget.h"
#include "ColorWidget.h"

class Label : public virtual PositionedWidget, public virtual ColorWidget {
private:
    std::string text;

    // time for update
    bool autoUpdateMode = false;
    std::chrono::duration<double> delta{1};
    std::chrono::time_point<std::chrono::system_clock> start;

    // Now we can make regular update name of Label class
    std::function<std::string()> newName;
public:
    Label(const std::string &name, const std::string &text);

    void show() override;

    void changeText(std::string next) { text = std::move(next); }

    void setRegularNameChanging(std::chrono::milliseconds delta, std::function<std::string()> rename);

    UISize getSize() override {
        if (isHidden)
            return UISize{0, 0};
        return size;
    }
};
