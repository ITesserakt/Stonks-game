#pragma clang diagnostic push
#pragma ide diagnostic   ignored "google-explicit-constructor"
//
// Created by tesserakt on 06.12.2021.
//

#include <string>
#include <utility>

#include "Canvas.h"
#include "Command.h"
#include "widgets/Button.h"
#include "widgets/Graphic.h"
#include "widgets/Group.h"
#include "widgets/Label.h"
#include "widgets/MessageBox.h"

#pragma once

namespace widget {
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
            if constexpr (sizeof...(Ts) == 0) return;
            else
                foreach
                    <sizeof...(Ts) - 1, F, Ts...>(tuple, action);
        }

        template <typename R, typename T, typename... Ts>
        constexpr bool hasArgOfType() {
            if constexpr (std::is_same<T, R>::value) {
                return true;
            } else if constexpr (sizeof...(Ts) > 0) {
                return hasArgOfType<R, Ts...>();
            } else {
                return false;
            }
        }

        template <typename T, typename... Args, typename Dummy = std::enable_if_t<std::is_constructible_v<T, Args...>>>
        T ctor(Args &&...args) {
            return T(std::forward<Args &&>(args)...);
        }
    }// namespace __detail

    namespace setup {
        template <typename W>
        struct DSLSetup {
            virtual void effect(W &widget) = 0;
        };

        struct hide : DSLSetup<PositionedWidget> {
            bool h;
            hide(bool hide) : h(hide) {}
            void effect(PositionedWidget &widget) override {
                widget.hide(h);
            }
        };

        template <typename C>
        struct command : DSLSetup<HoverableWidget> {
            C cmd;
            template <typename... Args>
            command(Args &&...cmd) : cmd(std::forward<Args>(cmd)...) {}
            void effect(HoverableWidget &widget) override {
                widget.applyAction(std::move(cmd));
            }
        };

        template <typename C, typename... Args>
        auto sender_command(Args &&...args) {
            using T = typename C::widget_type;

            static_assert(std::is_constructible_v<C, T &, Args...> || std::is_constructible_v<C, Args...>,
                    "Cannot find appropriate constructor. Sender param should be the first in the chosen parameters "
                    "list");

            struct sender_command_helper : DSLSetup<T> {
                std::tuple<Args &&...> args;
                sender_command_helper(Args &&...args) : args(std::forward<Args>(args)...) {}
                void effect(T &widget) override {
                    if constexpr (__detail::hasArgOfType<T, Args...>()) {
                        auto obj = std::apply(__detail::ctor<C, Args &&...>, args);
                        widget.applyAction(std::move(obj));
                    } else {
                        auto obj = std::apply(__detail::ctor<C, T &, Args &&...>,
                                std::tuple_cat(std::tuple<T &>{widget}, std::move(args)));
                        widget.applyAction(std::move(obj));
                    }
                }
            };

            return sender_command_helper(std::forward<Args &&>(args)...);
        }

        struct color : DSLSetup<ColorWidget> {
            int c;
            color(int color) : c(color) {}
            void effect(ColorWidget &widget) override {
                widget.turnOn(c);
            }
        };

        template <typename F>
        struct withUpdate : DSLSetup<Label> {
            F                         onUpdate;
            std::chrono::milliseconds duration;
            withUpdate(std::chrono::milliseconds duration, F onUpdate) : onUpdate(onUpdate), duration(duration) {}
            void effect(Label &widget) override {
                widget.setRegularNameChanging(duration, std::move(onUpdate));
            }
        };

        template <typename W>
        struct copyTo : DSLSetup<W> {
            std::shared_ptr<W> *nullable;
            copyTo(std::shared_ptr<W> *nullable) : nullable(nullable) {}
            void effect(W &widget) override {
                *nullable = widget;
            }
        };

        template <typename H>
        struct auto_index {
            std::size_t index = 0;

            std::size_t operator()() {
                return index++;
            }
        };
    }// namespace setup

    template <typename W>
    struct Holder {
        std::shared_ptr<W> constructed;

        template <typename... Args, typename... Defaults>
        explicit Holder(std::tuple<Defaults...> &&def, Args &&...args)
            : constructed(std::make_shared<W>(std::forward<Args &&>(args)...)) {
            __detail::tupleForeach(def, [this](auto &d) { d.effect(*constructed); });
        }

        Holder(std::shared_ptr<W> widget) : constructed(std::move(widget)) {}

        operator std::shared_ptr<W>() {
            return constructed;
        }

        W *operator->() {
            return constructed.operator->();
        }

        template <typename Default>
        Holder<W> &operator<<(Default &&def) {
            def.effect(*constructed);
            return *this;
        }
    };

    struct button : Holder<Button> {
        template <typename... Defaults>
        button(const std::string &name, unsigned int index, Defaults &&...def)
            : Holder<Button>(std::tuple<Defaults &&...>{std::forward<Defaults &&>(def)...}, name, index) {}

        template <typename... Defaults>
        button(const std::string &name, setup::auto_index<button> &idx, Defaults &&...def)
            : button(name, idx(), std::forward<Defaults &&>(def)...) {}
    };

    struct label : Holder<Label> {
        template <typename... Defaults>
        label(const std::string &name, const std::string &text, Defaults &&...def)
            : Holder<Label>(std::tuple<Defaults &&...>{std::forward<Defaults &&>(def)...}, name, text) {}
    };

    struct graphic : Holder<Graphic> {
        template <typename... Defaults>
        graphic(const std::string &name, const std::string &abscissa, const std::string &ordinance, UISize size,
                const std::function<double()> &valueSink, Defaults &&...def)
            : Holder<Graphic>(std::tuple<Defaults &&...>{std::forward<Defaults &&>(def)...}, name, ordinance, abscissa,
                      size, valueSink) {}
    };

    struct message_box : Holder<MessageBox> {
        template <typename... Defaults>
        message_box(const std::string &name, const std::string &text, SpecialPosition position, Defaults &&...def)
            : Holder<MessageBox>(std::tuple<Defaults &&...>{std::forward<Defaults &&>(def)...}, name, text, position) {}
    };

    struct canvas : Holder<Canvas> {
        template <typename... Widgets>
        canvas(std::string name, Align align, Widgets... widgets) : Holder<Canvas>({}, name, align) {
            (constructed->bind(widgets.constructed), ...);
        }

        canvas(std::shared_ptr<Canvas> c) : Holder<Canvas>(std::move(c)) {}

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

    template <typename W>
    struct many {
        std::vector<std::shared_ptr<W>> constructed;

        template <typename... Defaults, typename F = W(std::size_t &)>
        many(std::size_t count, F fn = __detail::ctor<W, std::size_t &>) : constructed(count) {
            for (std::size_t i = 0; i < count; i++) constructed[i] = std::make_shared<W>(fn(i));
        }

        template <typename Default>
        many &operator<<(Default &&def) {
            for (auto &item : constructed) def.effect(*item);
            return *this;
        }
    };

    template <typename F, typename... Def>
    many(std::size_t, F, Def...) -> many<Def..., std::invoke_result_t<F, std::size_t>>;
}// namespace widget
#pragma clang diagnostic pop