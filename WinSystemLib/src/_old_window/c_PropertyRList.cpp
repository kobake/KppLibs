#include <BaseLibOld.h>
#include "c_PropertyRList.h"

void SimpleList_newValue(Property* &dst)
{
	dst=NULL;
}

void SimpleList_copyValue(Property* &dst,Property* &src)
{
	dst=src;
}

void SimpleList_deleteValue(Property* &dst)
{
	//何もしない
}

