#include <BaseLibOld.h>
#include "c_Menu.h"

OldMenu *CreateDefMenu(MENUDEF *list,bool popup)
{
	OldMenu *tmp[16]; int itmp=0;
	if(popup){
		tmp[0]=new PopupMenu();
	}else{
		tmp[0]=new OldMenu();
	}
	for(int i=0;(list[i].state & KMENU_END) == 0;i++){
		tmp[itmp]->insertItem(-1,list[i].name,list[i].id,list[i].state);
		if(list[i].state & KMENU_POPUP){
			tmp[itmp+1]=tmp[itmp]->setSubMenu(-1);
			itmp++;
		}else if(list[i].state & KMENU_POPEND){
			itmp--;
		}
	}
	return tmp[0];
}

OldMenu *CreateDefMenu(MENUDEF *list,int nlist)
{
	OldMenu *tmp[16]; int itmp=0;
	tmp[0]=new OldMenu();
	for(int i=0;i<nlist;i++){
		tmp[itmp]->insertItem(-1,list[i].name,list[i].id,list[i].state);
		if(list[i].state & KMENU_POPUP){
			tmp[itmp+1]=tmp[itmp]->setSubMenu(-1);
			itmp++;
		}else if(list[i].state & KMENU_POPEND){
			itmp--;
		}
	}
	return tmp[0];
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- --  コンストラクタ・デストラクタ  -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

OldMenu::OldMenu(HMENU _hMenu)
{
	hMenu=_hMenu;
	hMenu_is_created=false;
	submenu=NULL;
	nsubmenu=0;
}

OldMenu::OldMenu()
{
	hMenu=CreateMenu();
	hMenu_is_created=true;
	submenu=NULL;
	nsubmenu=0;
}

PopupMenu::PopupMenu()
{
	hMenu=CreatePopupMenu();
	submenu=NULL;
	nsubmenu=0;
}

OldMenu::~OldMenu()
{
	for(int i=0;i<nsubmenu;i++)setSubMenu(i,NULL);
	free(submenu); submenu=NULL;
	if(hMenu_is_created){
		DeleteObject(hMenu);
	}
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- 環境設定 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void OldMenu::adjustTo(Window *wnd)
{
	SetMenu(wnd->getHWND(),hMenu);
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- 項目管理 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

BOOL OldMenu::insertItem(int index,const wchar *new_caption,int new_id,int _option)
{
	int nitem=GetMenuItemCount(hMenu);
	if(index==-1)index=nitem;
	if(index<0 || index>nitem)return FALSE;
	MENUITEMINFO mii;
	mii.cbSize=sizeof(MENUITEMINFO);
	if(wcscmp(new_caption,L"-")==0){
		mii.fMask=MIIM_TYPE;
		mii.fType=MFT_SEPARATOR;
		mii.dwTypeData=NULL;
	}else{
		mii.fMask=MIIM_TYPE | MIIM_ID | MIIM_STATE;
		mii.fType=MFT_STRING;
		mii.dwTypeData=(wchar*)new_caption;
		mii.wID=new_id;
		mii.fState=0;
		if(_option & KMENU_GRAYED){
			mii.fState|=MFS_GRAYED;
		}
	}
	BOOL ret=InsertMenuItem(hMenu,index,TRUE,&mii);
	if(ret){
		nsubmenu++;
		submenu=(OldMenu**)realloc(submenu,sizeof(OldMenu*)*nsubmenu);
		memmove(&submenu[index+1],&submenu[index],sizeof(OldMenu*)*(nsubmenu-index-1));
		submenu[index]=NULL;
	}
	return ret;
}



OldMenu *OldMenu::setSubMenu(int index,OldMenu *_submenu)
{
	if(index==-1){
		index=nsubmenu-1;
	}
	if(_submenu!=NULL){
		MENUITEMINFO mmi;
		mmi.cbSize=sizeof(MENUITEMINFO);
		mmi.fMask=MIIM_SUBMENU;
		mmi.hSubMenu=_submenu->hMenu;
		if(SetMenuItemInfo(hMenu,index,TRUE,&mmi)){
			if(submenu[index]!=NULL)delete submenu[index]; //###メニューの挿げ替え時に古いものをdeleteすべきか微妙
			submenu[index]=_submenu;
			return _submenu;
		}else{
			return NULL;
		}
	}else{
		MENUITEMINFO mmi;
		mmi.cbSize=sizeof(MENUITEMINFO);
		mmi.fMask=MIIM_SUBMENU;
		mmi.hSubMenu=NULL;
		if(SetMenuItemInfo(hMenu,index,TRUE,&mmi)){
			delete submenu[index]; //###親側からdeleteしまって良いか微妙
			submenu[index]=NULL;
			return NULL;
		}else{
			return NULL;
		}
	}
}

OldMenu *OldMenu::setSubMenu(int index)
{
	OldMenu *new_submenu=new OldMenu();
	OldMenu *ret=setSubMenu(index,new_submenu);
	if(ret==NULL){
		delete new_submenu;
	}
	return ret;
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- 項目操作 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void OldMenu::enableItemByID(int id,bool b)
{
	EnableMenuItem(hMenu,id,MF_BYCOMMAND | (b?MF_ENABLED:MF_GRAYED));
}
void OldMenu::checkItemByID(int id,bool b)
{
	CheckMenuItem(hMenu,id,MF_BYCOMMAND | (b?MF_CHECKED:MF_UNCHECKED));
}

void OldMenu::setItemTextByID(int id,const wchar *text)
{
	MENUITEMINFO mii;
	mii.cbSize=sizeof(MENUITEMINFO);
	mii.fMask=MIIM_TYPE;
	mii.fType=MFT_STRING;
	mii.dwTypeData=(wchar*)text;
	SetMenuItemInfo(hMenu,id,FALSE,&mii);
}

const wchar* OldMenu::getItemTextByID(int id)
{
	static wchar buf[256];
	MENUITEMINFO mii;
	mii.cbSize=sizeof(MENUITEMINFO);
	mii.fMask=MIIM_TYPE;
	mii.fType=MFT_STRING;
	mii.dwTypeData=buf;
	mii.cch=sizeof(buf);
	if(SetMenuItemInfo(hMenu,id,FALSE,&mii)){
		return buf;
	}else{
		return NULL;
	}
}




// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- 動作  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

int OldMenu::popup(Window *owner,bool sync)
{
	POINT pt; GetCursorPos(&pt);
	UINT flag=
		TPM_LEFTALIGN | TPM_TOPALIGN | 
		TPM_LEFTBUTTON | TPM_RIGHTBUTTON;
	if(sync){
		flag|=TPM_RETURNCMD | TPM_NONOTIFY;
	}
	SetForegroundWindow(owner->getHWND());
	return TrackPopupMenu(hMenu,flag,pt.x,pt.y,0,owner->getHWND(),NULL);
}

