#include "game_widgets/SharedGraphic.h"
#include "Debug.h"
#include "game_widgets/TradeButton.h"

void SharedGraphic::updateData(double num) {
    auto current = state.getCurrentScene().getActiveWidget();
    if (current->is<TradeButton>()) {
        if (!current->as<TradeButton>()->isObjectExist()) {
            values.clear();
            return;
        }
        auto list = Stat::ItemStat::getValuesById(current->as<TradeButton>()->getItemId());
        if (list != nullptr) {
            values = *list;
            // clearing unnecessary data
            while (values.size() > amountOfColumns) { values.pop_front(); }
        }
    }
    else {
        values.clear();
    }
}
