#pragma once

#include "PositionedWidget.h"
#include "BindableWidget.h"

struct Group : virtual PositionedWidget, virtual BindableWidget {
    UISize getSize() override;

    void show() override;

    explicit Group(const std::string &name);

    void bind(std::shared_ptr<Widget> w) override {
        BindableWidget::bind(w);
    }

    template <typename W>
    void bind(std::vector<std::shared_ptr<W>> widgets) {
        for (const auto &widget : std::move(widgets)) bind(widget);
    }
};