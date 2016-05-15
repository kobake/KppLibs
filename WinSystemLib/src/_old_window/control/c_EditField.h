class EditField;

#ifndef _EditField_
#define _EditField_

#include "c_EditBase.h"

class EditField : public EditBase{
public:
	EditField(int x,int y,int w,int h,Window *_parent,int _option=0,int _id=UNIQUE_ID);
	virtual ~EditField();
};





#endif
