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
	//����
	SelectObject(hdc,hpnOrg);

	//�j��
	DeleteObject(hpnSelect);
}

void CDcPen::_update()
{
	const State& state=getState();

	//�V�����쐬���A�I��
	HPEN hpnNew=CreatePen(
		state.style==Inside?PS_INSIDEFRAME:PS_SOLID,
		state.width,
		state.color.win_color()
	);
	HPEN hpnOld=(HPEN)SelectObject(hdc,hpnNew);

	//����
	if(hpnOrg==NULL){
		hpnOrg=hpnOld;
		hpnSelect=hpnNew;
	}
	//����ȍ~
	else{
		DeleteObject(hpnSelect);
		hpnSelect=hpnNew;
	}
}


