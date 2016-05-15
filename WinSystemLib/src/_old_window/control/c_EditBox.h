class EditBox;

#ifndef _EditBox_
#define _EditBox_

#include "c_EditBase.h"

class EditBox : public EditBase{
public:
	EditBox(int x,int y,int w,int h,Window *_parent,int _option=0,int _id=UNIQUE_ID);
	virtual ~EditBox();
	//
};





#endif
