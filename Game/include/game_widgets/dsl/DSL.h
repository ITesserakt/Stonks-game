//
// Created by tesserakt on 12.12.2021.
//

#pragma once

#include "game_widgets/SharedGraphic.h"
#include "widgets/dsl/DSL.h"

namespace widget {
    struct shared_graphic : Holder<SharedGraphic> {
        template <typename... Defaults>
        shared_graphic(const std::string &name, const std::string &abscissa, const std::string &ordinance, UISize size,
                WorldState &state, Defaults &&...defs)
            : Holder<SharedGraphic>(std::tuple<Defaults &&...>(std::forward<Defaults>(defs)...), name, abscissa,
                      ordinance, size, state) {}
    };
}// namespace widget