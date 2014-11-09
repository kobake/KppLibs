#include "_required.h"
#include "Menu.h"


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//               コンストラクタ・デストラクタ                  //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

Menu::Menu(HINSTANCE hInst,int id)
{
	hMenu=LoadMenu(hInst,MAKEINTRESOURCE(id));
}

Menu::~Menu()
{
	DestroyMenu(hMenu);
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                     インターフェース                        //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void Menu::popup(HWND hwnd)
{
	//サブメニューを取得
	HMENU hSubMenu=GetSubMenu(hMenu,0);

	//カーソル位置取得
	POINT pt;
	GetCursorPos(&pt);

	//メニュー表示
	TrackPopupMenu(
		hSubMenu,
		TPM_BOTTOMALIGN, //メニュー下端を基準とする
		pt.x,pt.y,       //座標
		0,               //予約済み
		hwnd,            //オーナー
		NULL             //無視される
	);
}

