//
// Created by tesserakt on 21.11.2021.
//

#pragma once

#include <memory>
#include <utility>

class Command {
public:
    virtual void act() = 0;

    virtual ~Command() = default;

    static auto noop() {
        struct NoOpCommand : Command {
            void act() override {}
        };

        return NoOpCommand();
    }

    template <typename F>
    static auto fromFunction(F &&fn);
};

template <typename Self>
struct CloneCommand : virtual Command {
    virtual Self clone() const;

    template <typename C>
    auto then(CloneCommand<C> &&cmd) &&;

    template <typename C>
    auto operator+(C cmd) && {
        return std::move(*this).then(std::move(cmd));
    }
};

template <typename T>
class WidgetCommand : public virtual Command {
protected:
    T &sender;

public:
    using widget_type = T;

    explicit WidgetCommand(T &sender) : sender(sender) {}
};

template <typename T>
struct UpdateCommand : public virtual WidgetCommand<T> {
    virtual void update() = 0;
};

template <typename Self, typename C>
struct ChainCommand : CloneCommand<ChainCommand<Self, C>> {
    Self a;
    C    b;

    ChainCommand(CloneCommand<Self> &&a, CloneCommand<C> &&b) : a(a.clone()), b(b.clone()) {}

    void act() override {
        a.act();
        b.act();
    }
};

template <typename F>
struct FnCommand : CloneCommand<FnCommand<F>> {
    F f;

    explicit FnCommand(F &&f) : f(std::forward<F>(f)) {}

    void act() override {
        return f();
    }
};

template <typename F>
auto Command::fromFunction(F &&fn) {
    return FnCommand(std::forward<F>(fn));
}

template <typename Self>
template <typename C>
auto CloneCommand<Self>::then(CloneCommand<C> &&cmd) && {
    return ChainCommand(std::move(*this), std::forward<CloneCommand<C>>(cmd));
}

template <typename Self>
Self CloneCommand<Self>::clone() const {
    return Self{*dynamic_cast<const Self *>(this)};
}