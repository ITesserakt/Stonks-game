#pragma clang diagnostic push
#pragma ide diagnostic   ignored "google-explicit-constructor"
//
// Created by tesserakt on 06.12.2021.
//

#include <any>
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
    enum Resolution { Lazy, Eager };

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
            foreach (std::tuple<Ts...> &tuple, F action) { action(std::move(std::get<0>(tuple))); }
        };

        template <typename F, typename... Ts>
        void tupleForeach(std::tuple<Ts...> &tuple, F action) {
            if constexpr (sizeof...(Ts) == 0) return;
            else
                foreach
                    <sizeof...(Ts) - 1, F, Ts...>(tuple, action);
        }

        template <class TCallback, class TSourceTuple>
        auto mapTuple(TCallback &&callback, const TSourceTuple &sourceTuple);

        namespace {
            template <class TCallback, class TSourceTuple, std::size_t... Indices>
            auto mapTuple_(TCallback &&callback, const TSourceTuple &sourceTuple, std::index_sequence<Indices...>) {
                return std::forward_as_tuple(callback(std::get<Indices>(sourceTuple))...);
            }
        }// namespace

        template <class TCallback, class TSourceTuple>
        auto mapTuple(TCallback &&callback, const TSourceTuple &sourceTuple) {
            return mapTuple_(callback, sourceTuple, std::make_index_sequence<std::tuple_size<TSourceTuple>::value>());
        }

        template <typename R, typename T, typename... Ts>
        constexpr bool hasArgOfType() {
            if constexpr (std::is_same<T, R>::value) { return true; }
            else if constexpr (sizeof...(Ts) > 0) {
                return hasArgOfType<R, Ts...>();
            }
            else {
                return false;
            }
        }

        template <typename R, typename T, typename... Ts>
        constexpr bool hasArgWithSupertype() {
            using Derived = std::remove_reference_t<T>;
            using Base    = std::remove_reference_t<R>;
            if constexpr (std::is_base_of<Base, Derived>::value) { return true; }
            else if constexpr (sizeof...(Ts) > 0) {
                return hasArgOfType<R, Ts...>();
            }
            else {
                return false;
            }
        }

        template <typename T, typename... Args, typename Dummy = std::enable_if_t<std::is_constructible_v<T, Args...>>>
        T ctor(Args &&...args) {
            return T(std::forward<Args &&>(args)...);
        }

        constexpr Resolution operator&&(Resolution lhs, Resolution rhs) {
            if (lhs == Lazy || rhs == Lazy) return Lazy;
            return Eager;
        }
    }// namespace __detail

    namespace implicit {
        enum SearchScope { Root, FirstBindable, Bindable };

        template <Resolution type>
        struct DSLImplicit {
            constexpr static auto resolution_type = type;
        };

        template <typename W>
        struct self : DSLImplicit<Eager> {
            W &inject(Widget &self) {
                return *self.as<W>();
            }
        };

        template <typename W, SearchScope scope = Root>
        struct search : DSLImplicit<Lazy> {
            static constexpr bool lazy = true;
            const std::string    &name;
            std::string           bindableName;

            search(const std::string &name) : name(name) {}
            search(const std::string &name, std::string bindableName)
                : search(name), bindableName(std::move(bindableName)) {}

            template <typename B, typename F = bool(B &)>
            B &findParentBindable(
                    Widget &current,
                    F       condition = [](auto &_) { return true; }) {
                if (current.getParent().expired()) {
                    throw std::runtime_error(
                            "Parent widget of " + current.getName() + " had expired or current widget is root");
                }
                else {
                    auto parent = current.getParent().lock();
                    if (parent == nullptr)
                        throw std::runtime_error(
                                "Cannot find canvas in parents of " + current.getName() + " because it's root");
                    if (parent->is<B>() && condition(*parent->as<B>())) return *parent->as<B>();
                    else
                        return findParentBindable<B>(*parent);
                }
            }

            W &inject(Widget &self) {
                if constexpr (scope == Root)
                    return *findParentBindable<Canvas>(self).getChildWithName(name)->template as<W>();
                else if constexpr (scope == FirstBindable)
                    return *findParentBindable<BindableWidget>(self).getChildWithName(name)->template as<W>();
                else
                    return *findParentBindable<BindableWidget>(
                            self, [this](auto &b) { return b.getName() == this->bindableName; })
                                    .getChildWithName(name)
                                    ->template as<W>();
            }
        };

        template <typename T>
        constexpr auto is_implicit_v = std::is_base_of_v<DSLImplicit<Lazy>, std::remove_reference_t<T>> ||
                                       std::is_base_of_v<DSLImplicit<Eager>, std::remove_reference_t<T>>;

        template <typename Tu>
        auto resolveImplicits(Widget &scope, Tu &&args) {
            auto mapFn = [&scope](auto &&arg) -> decltype(auto) {
                if constexpr (implicit::is_implicit_v<decltype(arg)>) return arg.inject(scope);
                else
                    return std::forward<decltype(arg)>(arg);
            };
            return __detail::mapTuple(mapFn, args);
        }
    }// namespace implicit

    namespace setup {
        struct DSLSetupErased {
            virtual void effectErased(std::shared_ptr<Widget> widget) = 0;
            template <typename W>
            void effectErased(std::vector<std::shared_ptr<W>> widgets) {
                for (auto &item : widgets) effectErased(item->template as<Widget>());
            }
        };

        template <typename W, Resolution type>
        struct DSLSetup : DSLSetupErased {
            constexpr static auto resolution_type = type;

            virtual void effect(W &widget) = 0;
            virtual ~DSLSetup()            = default;

        private:
            void effectErased(std::shared_ptr<Widget> widget) final {
                effect(*widget->template as<W>());
            }
        };

        struct hide : DSLSetup<PositionedWidget, Eager> {
            bool h;
            hide(bool hide) : h(hide) {}
            void effect(PositionedWidget &widget) override {
                widget.hide(h);
            }
        };

        namespace __detail {
            template <typename C, Resolution type, typename... Args>
            struct command_helper : DSLSetup<HoverableWidget, type> {
                std::tuple<Args &&...> args;
                command_helper(Args &&...cmd) : args(std::forward<Args>(cmd)...) {}
                command_helper(std::tuple<Args &&...> &&args) : args(std::move(args)) {}
                void effect(HoverableWidget &widget) override {
                    auto cmd = std::make_from_tuple<C>(implicit::resolveImplicits(widget, args));
                    widget.applyAction(std::move(cmd));
                }

                template <typename C2, Resolution type2, typename... Ts>
                auto operator+(command_helper<C2, type2, Ts...> &&other) && {
                    constexpr auto combinedType = widget::__detail::operator&&(type, type2);
                    struct combine : command_helper<C, combinedType, Args...> {
                        std::tuple<Ts &&...> otherArgs;
                        combine(std::tuple<Args &&...> &&args, std::tuple<Ts &&...> &&otherArgs)
                            : command_helper<C, combinedType, Args...>(std::move(args)),
                              otherArgs(std::move(otherArgs)) {}

                        void effect(HoverableWidget &widget) override {
                            auto cmd1 = std::make_from_tuple<C>(implicit::resolveImplicits(widget, args));
                            auto cmd2 = std::make_from_tuple<C2>(implicit::resolveImplicits(widget, otherArgs));
                            widget.applyAction(std::move(cmd1) + std::move(cmd2));
                        }
                    };
                    return combine{std::move(args), std::move(other.args)};
                }
            };
        }// namespace __detail

        template <typename C, typename... Args>
        auto command(Args &&...args) {
            return __detail::command_helper<C, Lazy, Args...>{std::forward<Args &&>(args)...};
        }

        struct command_fn : DSLSetup<HoverableWidget, Eager> {
            std::function<void()> fn;

            template <typename F>
            command_fn(F fn) : fn(fn) {}

            void effect(HoverableWidget &widget) override {
                widget.applyAction(Command::fromFunction(fn));
            }
        };

        struct color : DSLSetup<ColorWidget, Eager> {
            int c;
            color(int color) : c(color) {}
            void effect(ColorWidget &widget) override {
                widget.turnOn(c);
            }
        };

        template <typename F>
        struct withUpdate : DSLSetup<Label, Lazy> {
            F                         onUpdate;
            std::chrono::milliseconds duration;
            withUpdate(std::chrono::milliseconds duration, F onUpdate) : onUpdate(onUpdate), duration(duration) {}
            void effect(Label &widget) override {
                widget.setRegularNameChanging(duration, std::move(onUpdate));
            }
        };

        template <typename W>
        struct copyTo : DSLSetup<W, Eager> {
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
        std::shared_ptr<W>                                                       constructed;
        std::vector<std::pair<std::shared_ptr<Widget>, setup::DSLSetupErased *>> lazyDefaults;
        using widget_type = W;

        template <typename... Args, typename... Defaults>
        explicit Holder(std::tuple<Defaults...> &&def, Args &&...args)
            : constructed(std::make_shared<W>(std::forward<Args &&>(args)...)) {
            __detail::tupleForeach(def, [this](auto &&d) {
                using arg_type = std::remove_reference_t<decltype(d)>;
                if constexpr (arg_type::resolution_type == Eager) d.effect(*constructed);
                else if constexpr (arg_type::resolution_type == Lazy) {
                    auto copy = new arg_type(std::forward<decltype(d)>(d));
                    lazyDefaults.emplace_back(constructed, copy);
                }
                else
                    throw std::runtime_error("Cannot defer class, not extended from DSLSetup");
            });
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
        button(const std::string &name, setup::auto_index<Button> &idx, Defaults &&...def)
            : button(name, idx(), std::forward<Defaults &&>(def)...) {}
    };

    struct label : Holder<Label> {
        template <typename... Defaults>
        label(const std::string &name, const std::string &text, Defaults &&...def)
            : Holder<Label>(std::tuple<Defaults &&...>{std::forward<Defaults &&>(def)...}, name, text) {}
    };

    struct graphic : Holder<Graphic> {
        template <typename... Defaults>
        graphic(const std::string             &name,
                const std::string             &abscissa,
                const std::string             &ordinance,
                UISize                         size,
                const std::function<double()> &valueSink,
                Defaults &&...def)
            : Holder<Graphic>(std::tuple<Defaults &&...>{std::forward<Defaults &&>(def)...},
                      name,
                      abscissa,
                      ordinance,
                      size,
                      valueSink) {}
    };

    struct message_box : Holder<MessageBox> {
        template <typename... Defaults>
        message_box(const std::string &name, const std::string &text, SpecialPosition position, Defaults &&...def)
            : Holder<MessageBox>(std::tuple<Defaults &&...>{std::forward<Defaults &&>(def)...}, name, text, position) {}
    };

    struct canvas : Holder<Canvas> {
        template <typename... Widgets>
        canvas(std::string name, Align align, Widgets &&...widgets) : Holder<Canvas>({}, name, align) {
            (constructed->bind(widgets.constructed), ...);
            (std::for_each(widgets.lazyDefaults.begin(), widgets.lazyDefaults.end(),
                     [&widgets](auto d) { d.second->effectErased(d.first); }),
                    ...);
        }

        canvas(std::shared_ptr<Canvas> c) : Holder<Canvas>(std::move(c)) {}

        template <typename... Widgets>
        void append(Widgets... widgets) {
            (constructed->bind(widgets.constructed), ...);
            (std::for_each(widgets.lazyDefaults.begin(), widgets.lazyDefaults.end(),
                     [&widgets](auto d) { d.second->effectErased(d.first); }),
                    ...);
        }
    };

    struct group : Holder<Group> {
        template <typename... Widgets>
        explicit group(std::string name, Widgets... widgets) : Holder<Group>({}, name) {
            (constructed->bind(widgets.constructed), ...);
            (lazyDefaults.insert(lazyDefaults.end(), widgets.lazyDefaults.begin(), widgets.lazyDefaults.end()), ...);
        }
    };

    template <typename W>
    struct many {
        using widget_type = W;
        std::vector<std::shared_ptr<W>>                                          constructed;
        std::vector<std::pair<std::shared_ptr<Widget>, setup::DSLSetupErased *>> lazyDefaults;

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