class Menu;
class PopupMenu;

#ifndef _Menu_
#define _Menu_

#include "c_Window.h"

class Menu{
protected:
	HMENU hMenu;
	bool hMenu_is_created;
	Menu **submenu;
	int nsubmenu;
public:
	//コンストラクタ・デストラクタ
	Menu(HMENU _hMenu);
	Menu();
	virtual ~Menu();
	//環境設定
	void adjustTo(Window *wnd);
	//項目管理
	BOOL insertItem(int index,const wchar *new_caption,int new_id,int _option=0);
	Menu *setSubMenu(int index,Menu *_submenu);
	Menu *setSubMenu(int index);
	//項目操作
	void		enableItemByID(int id,bool b);
	void		checkItemByID(int id,bool b);
	void		setItemTextByID(int id,const wchar *text);
	const wchar*	getItemTextByID(int id);
	//動作
	int popup(Window *owner,bool sync=true);
};

class PopupMenu : public Menu{
public:
	PopupMenu();
};


// -- -- -- -- メニュー簡易作成関数 -- -- -- -- //

#define KMENU_POPUP		0x001
#define KMENU_POPEND	0x002
#define KMENU_END		0x010
#define KMENU_GRAYED	0x100

struct MENUDEF{
	wchar *name;
	int id;
	int state;
};

//Menu *CreateDefMenu(MENUDEF *list,int nlist);
Menu *CreateDefMenu(MENUDEF *list,bool popup);


#endif
