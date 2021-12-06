#pragma clang diagnostic push
#pragma ide diagnostic ignored "google-explicit-constructor"
//
// Created by tesserakt on 06.12.2021.
//

#include <string>
#include <utility>

#include "Canvas.h"
#include "widgets/Button.h"
#include "widgets/Graphic.h"
#include "widgets/Group.h"
#include "widgets/Label.h"
#include "widgets/MessageBox.h"

#pragma once

namespace widget {
    namespace setup {
        template <typename W>
        struct DSLSetup {
            virtual void effect(W &widget) = 0;
        };

        struct hide : DSLSetup<PositionedWidget> {
            bool h;
            hide(bool hide) : h(hide) { }
            void effect(PositionedWidget &widget) override { widget.hide(h); }
        };

        template <typename C>
        struct command : DSLSetup<HoverableWidget> {
            typename std::remove_reference<C>::type cmd;
            command(C &&cmd) : cmd(std::forward<C>(cmd)) { }
            void effect(HoverableWidget &widget) override { widget.applyAction(std::move(cmd)); }
        };

        struct color : DSLSetup<ColorWidget> {
            int c;
            color(int color) : c(color) { }
            void effect(ColorWidget &widget) override { widget.turnOn(c); }
        };

        template <typename F>
        struct withUpdate : DSLSetup<Label> {
            typename std::remove_reference<F>::type onUpdate;
            std::chrono::milliseconds duration;
            withUpdate(std::chrono::milliseconds duration, F &&onUpdate)
                : onUpdate(std::forward<F>(onUpdate)),
                  duration(duration) { }
            void effect(Label &widget) override { widget.setRegularNameChanging(duration, onUpdate); }
        };

        template <typename W>
        struct copyTo : DSLSetup<W> {
            std::shared_ptr<W> *nullable;
            copyTo(std::shared_ptr<W> *nullable) : nullable(nullable) { }
            void effect(W &widget) override { *nullable = widget; }
        };
    }// namespace setup

    namespace __detail {
        template <std::size_t index, typename F, typename... Ts>
        struct foreach {
            foreach (std::tuple<Ts...> &tuple, F action) {
                action(std::get<index>(tuple));
                foreach
                    <index - 1, F, Ts...>(tuple, action);
            }
        };

        template <typename F, typename... Ts>
        struct foreach<0, F, Ts...> {
            foreach (std::tuple<Ts...> &tuple, F action) { action(std::get<0>(tuple)); }
        };

        template <typename F, typename... Ts>
        void tupleForeach(std::tuple<Ts...> &tuple, F action) {
            foreach
                <sizeof...(Ts) - 1, F, Ts...>(tuple, action);
        }
    }// namespace __detail

    template <typename W>
    struct Holder {
        std::shared_ptr<W> constructed;

        template <typename... Args, typename... Defaults>
        explicit Holder(std::tuple<Defaults...> def, Args &&...args)
            : constructed(std::make_shared<W>(std::forward<Args &&>(args)...)) {
            if constexpr (sizeof...(Defaults) > 0)
                __detail::tupleForeach(def, [this](auto &d) { d.effect(*constructed); });
        }

        Holder(std::shared_ptr<W> widget) : constructed(widget) { }

        operator std::shared_ptr<W>() { return constructed; }

        W *operator->() { return constructed.operator->(); }

        template <typename Default>
        Holder<W> &operator<<(Default &&def) {
            def.effect(*constructed);
            return *this;
        }
    };

    struct button : Holder<Button> {
        template <typename... Defaults>
        button(const std::string &name, unsigned int index, Defaults &&...def)
            : Holder<Button>(std::tuple<Defaults...>{ def... }, name, index) { }
    };

    struct label : Holder<Label> {
        template <typename... Defaults>
        label(const std::string &name, const std::string &text, Defaults &&...def)
            : Holder<Label>(std::tuple<Defaults...>{ def... }, name, text) { }
    };

    struct graphic : Holder<Graphic> {
        template <typename... Defaults>
        graphic(const std::string &name, const std::string &abscissa, const std::string &ordinance, UISize size,
                const std::function<double()> &valueSink, Defaults &&...def)
            : Holder<Graphic>(std::tuple<Defaults...>{ def... }, name, ordinance, abscissa, size, valueSink) { }
    };

    struct message_box : Holder<MessageBox> {
        template <typename... Defaults>
        message_box(const std::string &name, const std::string &text, SpecialPosition position, Defaults &&...def)
            : Holder<MessageBox>(std::tuple<Defaults...>{ def... }, name, text, position) { }
    };

    struct canvas : Holder<Canvas> {
        template <typename... Widgets>
        canvas(std::string name, Align align, Widgets... widgets) : Holder<Canvas>({}, name, align) {
            (constructed->bind(widgets.constructed), ...);
        }

        canvas(std::shared_ptr<Canvas> c) : Holder<Canvas>(std::move(c)) { }

        template <typename... Widgets>
        void append(Widgets... widgets) {
            (constructed->bind(widgets.constructed), ...);
        }
    };

    struct group : Holder<Group> {
        template <typename... Widgets>
        explicit group(std::string name, Widgets... widgets) : Holder<Group>({}, name) {
            (constructed->bind(widgets.constructed), ...);
        }
    };
}// namespace widget
#pragma clang diagnostic pop