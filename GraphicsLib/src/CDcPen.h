#pragma once

#include <windows.h>
#include "Pen.h"


class CDcPen : public Pen{
public:
	CDcPen(HDC _hdc);
	virtual ~CDcPen();
	void _update();
private:
	HDC hdc;
	HPEN hpnSelect,hpnOrg;
};

