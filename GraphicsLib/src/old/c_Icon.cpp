#include "include_sys.h"
#include "c_Icon.h"
#include "c_App.h"

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- --  �R���X�g���N�^�E�f�X�g���N�^  -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
Icon::Icon(App *app,int id,int w,int h)
{
//	hIcon=LoadIcon(app->getInstance(),MAKEINTRESOURCE(id));
	if(w==0 || h==0){
		hIcon=(HICON)LoadImage(app->getInstance(),MAKEINTRESOURCE(id),IMAGE_ICON,0,0,LR_DEFAULTCOLOR	| LR_SHARED | LR_DEFAULTSIZE);
	}else{
		hIcon=(HICON)LoadImage(app->getInstance(),MAKEINTRESOURCE(id),IMAGE_ICON,w,h,LR_DEFAULTCOLOR	| LR_SHARED);
	}
	is_system=true;
}
Icon::Icon(Icon *src)
{
	if(is_system){
		hIcon=src->hIcon; //�j������邱�Ƃ͂Ȃ��̂ŁA�l�����̂܂܃R�s�[
		is_system=true;
	}else{
		hIcon=CopyIcon(src->hIcon); //�N���[�����쐬
		is_system=false;
	}
}
Icon::~Icon()
{
	if(!is_system)
		DestroyIcon(hIcon);
}

Icon *Icon::clone()
{
	return new Icon(this);
}

