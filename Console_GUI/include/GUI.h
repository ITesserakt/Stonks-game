#pragma once

namespace console_gui {
    namespace __detail {
        struct Frontend {
        public:
            virtual void init() = 0;

            virtual void dispose() = 0;
        };

        struct NCursesFrontend;

        class TerminalFrontend;

        struct NoopFrontend;
    }// namespace __detail

    using NCurses = __detail::NCursesFrontend;
    using Terminal = __detail::TerminalFrontend;
    using Noop = __detail::NoopFrontend;

    template <typename T>
    void init() {
        static_assert(std::is_same<T, void>::value && !std::is_same<T, void>::value,
                      "Expected types from console_gui namespace");// impossible
    }

    template <>
    void init<NCurses>();

    template <>
    void init<Terminal>();

    template <>
    void init<Noop>();

    template <typename T>
    void dispose() {
        static_assert(std::is_same<T, void>::value && !std::is_same<T, void>::value,
                      "Expected types from console_gui namespace");// impossible
    }

    template <>
    void dispose<NCurses>();

    template <>
    void dispose<Terminal>();

    template <>
    void dispose<Noop>();
}// namespace console_gui