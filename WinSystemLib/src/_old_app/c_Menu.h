class OldMenu;
class PopupMenu;

#ifndef _Menu_
#define _Menu_

#include "../_old_window/c_Window.h"

class OldMenu{
protected:
	HMENU hMenu;
	bool hMenu_is_created;
	OldMenu **submenu;
	int nsubmenu;
public:
	//コンストラクタ・デストラクタ
	OldMenu(HMENU _hMenu);
	OldMenu();
	virtual ~OldMenu();
	//環境設定
	void adjustTo(Window *wnd);
	//項目管理
	BOOL insertItem(int index,const wchar *new_caption,int new_id,int _option=0);
	OldMenu *setSubMenu(int index,OldMenu *_submenu);
	OldMenu *setSubMenu(int index);
	//項目操作
	void		enableItemByID(int id,bool b);
	void		checkItemByID(int id,bool b);
	void		setItemTextByID(int id,const wchar *text);
	const wchar*	getItemTextByID(int id);
	//動作
	int popup(Window *owner,bool sync=true);
};

class PopupMenu : public OldMenu{
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

//OldMenu *CreateDefMenu(MENUDEF *list,int nlist);
OldMenu *CreateDefMenu(MENUDEF *list,bool popup);


#endif
