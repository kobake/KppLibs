class Label;

#ifndef DEF_LABEL
#define DEF_LABEL CTL_CLASS_BEGIN+6

#include "c_Window.h"

#define LABELF_ALIGN(option) (option & 0x0003)
#define LABELF_ALIGN_LEFT		0x0000 // 0000 0000 0000 0000
#define LABELF_ALIGN_RIGHT		0x0001 // 0000 0000 0000 0001
#define LABELF_ALIGN_VCENTER	0x0002 // 0000 0000 0000 0010

class Label : public Window{
public:
	Label(const wchar* caption,int x,int y,int w,int h,Window *_parent,int _option=0,int _id=UNIQUE_ID);//,int border=0,int align=0);
	virtual ~Label();
};





#endif
