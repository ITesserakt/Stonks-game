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
    }

    using NCurses = __detail::NCursesFrontend;
    using Terminal = __detail::TerminalFrontend;

    template<typename T>
    void init() {
        static_assert(std::is_same<T, void>::value && !std::is_same<T, void>::value,
                      "Expected types from console_gui namespace");// impossible
    }

    template<>
    void init<NCurses>();

    template<>
    void init<Terminal>();

    template<typename T>
    void dispose() {
        static_assert(std::is_same<T, void>::value && !std::is_same<T, void>::value,
                      "Expected types from console_gui namespace");// impossible
    }

    template<>
    void dispose<NCurses>();

    template<>
    void dispose<Terminal>();
}