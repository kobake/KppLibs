#include <BaseLib.h>
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
	//復元
	SelectObject(hdc,hpnOrg);

	//破棄
	DeleteObject(hpnSelect);
}

void CDcPen::_update()
{
	const State& state=getState();

	//新しく作成し、選択
	HPEN hpnNew=CreatePen(
		state.style==Inside?PS_INSIDEFRAME:PS_SOLID,
		state.width,
		state.color.win_color()
	);
	HPEN hpnOld=(HPEN)SelectObject(hdc,hpnNew);

	//初回
	if(hpnOrg==NULL){
		hpnOrg=hpnOld;
		hpnSelect=hpnNew;
	}
	//それ以降
	else{
		DeleteObject(hpnSelect);
		hpnSelect=hpnNew;
	}
}


