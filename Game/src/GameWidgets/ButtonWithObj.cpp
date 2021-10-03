#include "GameWidgets/ButtonWithObj.h"

UISize findSize(ButtonWithObj* but){
    but->size.height = getHeight(but->name);
    but->size.height += but->isBlowing and but->getItemId() != -1 ? 2 : 0;
    but->size.width = getWidth(but->name) + 4;
    return but->size;
}
