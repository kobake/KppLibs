#pragma once

#include <BaseLib.h> // t_SimpleList
#include "c_Property.h"

void SimpleList_newValue(Property* &dst);
void SimpleList_copyValue(Property* &dst,Property* &src);
void SimpleList_deleteValue(Property* &dst);


class PropertyRList : public SimpleList<Property*> {
};
