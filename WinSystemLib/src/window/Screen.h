#pragma once

#include "design/TSingleton.h"
#include "../../../MathLib/MathLib.h" //CRect

class Screen : public TSingleton<Screen>{
public:
	Screen();
	~Screen();
	const math::CRect& getRect() const{ return rc; }
	void update(); //!<�ŐV�̃X�N���[����ԂɍX�V
private:
	math::CRect rc;
};
