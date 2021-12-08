#include "game_widgets/SharedGraphic.h"
#include "game_widgets/Purchase.h"
#include "Debug.h"

void SharedGraphic::updateData(double num) {
    auto current = state.getCurrentScene().getActiveWidget();
    if (current->is<Purchase>()) {
        if (!current->as<Purchase>()->isObjectExist()) {
            values.clear();
            return;
        }
        auto list = Stat::ItemStat::getValuesById(current->as<Purchase>()->getItemId());
        if (list != nullptr) {
            values = *list;
            // clearing unnecessary data
            while (values.size() > amountOfColumns) {
                values.pop_front();
            }
        }
    }
}
