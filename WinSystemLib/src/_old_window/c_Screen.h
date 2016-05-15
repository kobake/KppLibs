//### 画面サイズよりデスクトップがでかい場合に対応する必要がある

class Screen;

#ifndef _Screen_
#define _Screen_

#include "c_Window.h"

class Screen : public Window{
public:
	Screen();
	virtual ~Screen();
};


#endif
