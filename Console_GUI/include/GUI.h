#pragma once

namespace console_gui {
    namespace __detail {
        struct Bootstrap {
        public:
            virtual void bootstrap() const = 0;
        };

        struct NCursesBootstrap : public Bootstrap {
            void bootstrap() const override;
        };

        struct TerminalBootstrap : public Bootstrap {
            void bootstrap() const override;
        };
    }

    using NCurses = __detail::NCursesBootstrap;
    using Terminal = __detail::TerminalBootstrap;

    template<typename T>
    void initGUI() {
        T().bootstrap();
    }
}