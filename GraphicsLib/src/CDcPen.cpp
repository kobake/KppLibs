#include "common/_required.h"
#include "CDcPen.h"


CDcPen::CDcPen(HDC _hdc)
: hdc(_hdc)
{
	hpnOrg=NULL;
	hpnSelect=NULL;
	_update();
}

CDcPen::~CDcPen()
{
	//•œŒ³
	SelectObject(hdc,hpnOrg);

	//”jŠü
	DeleteObject(hpnSelect);
}

void CDcPen::_update()
{
	const State& state=getState();

	//V‚µ‚­ì¬‚µA‘I‘ğ
	HPEN hpnNew=CreatePen(
		state.style==Inside?PS_INSIDEFRAME:PS_SOLID,
		state.width,
		state.color.win_color()
	);
	HPEN hpnOld=(HPEN)SelectObject(hdc,hpnNew);

	//‰‰ñ
	if(hpnOrg==NULL){
		hpnOrg=hpnOld;
		hpnSelect=hpnNew;
	}
	//‚»‚êˆÈ~
	else{
		DeleteObject(hpnSelect);
		hpnSelect=hpnNew;
	}
}


