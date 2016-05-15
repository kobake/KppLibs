class PropertyRList;

#ifndef DEF_PROPERTY_RLIST
#define DEF_PROPERTY_RLIST

#include "t_SimpleList.h"
#include "c_Property.h"

void SimpleList_newValue(Property* &dst);
void SimpleList_copyValue(Property* &dst,Property* &src);
void SimpleList_deleteValue(Property* &dst);


class PropertyRList : public SimpleList<Property*> {
};

#endif
