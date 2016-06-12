#include <BaseLib.h>
#include <BaseLibOld.h>
#include <windows.h>
#include <commctrl.h>
#include <string>
using namespace std;
#include "c_TreeItem.h"
#include <StringLib.h>
using namespace util;

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- --  コンストラクタ・デストラクタ  -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
void TreeItem::_init_v(TreeView *_window,HTREEITEM _hItem,LPARAM _lParam,bool _lParam_is_valid,const wchar *_text,bool _text_is_valid)
{
	window=_window;
	hItem=_hItem;
	lParam_is_valid=_lParam_is_valid;
	if(lParam_is_valid){
		lParam=_lParam;
	}else{
		lParam=0;
	}
	text_is_valid=_text_is_valid;
	if(text_is_valid){
		text=wcsdup(_text);
	}else{
		text=NULL;
	}
}

TreeItem::TreeItem(NMTREEVIEW *nm,int new_or_old)
{
	TreeView *_window=(TreeView*)GetWindowLong(nm->hdr.hwndFrom,GWL_USERDATA);
	if(new_or_old==NEW_ITEM){
		_init_v(_window,nm->itemNew.hItem,nm->itemNew.lParam,true);
	}else if(new_or_old==OLD_ITEM){
		_init_v(_window,nm->itemOld.hItem,nm->itemOld.lParam,true);
	}else{
		_init_v(_window,NULL,0,false);
	}
}
TreeItem::TreeItem(NMTVDISPINFO *di)
{
	TreeView *_window=(TreeView*)GetWindowLong(di->hdr.hwndFrom,GWL_USERDATA);
	_init_v(_window,di->item.hItem,di->item.lParam,true,di->item.pszText,true);
}

TreeItem::TreeItem(TreeView *_window,HTREEITEM _hItem)
{
	_init_v(_window,_hItem);
}
TreeItem::TreeItem(TreeView *_window,HTREEITEM _hItem,LPARAM _lParam)
{
	_init_v(_window,_hItem,_lParam,true);
}
TreeItem::TreeItem(TreeItem &right)
{
	_init_v(right.window,right.hItem,right.lParam,right.lParam_is_valid,right.text,right.text_is_valid);
}
TreeItem::TreeItem()
{
	_init_v(NULL,NULL,0,false);
}
TreeItem::~TreeItem()
{
	if(text_is_valid){
		free(text); text=NULL;
	}
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- --   操作   -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
TreeItem TreeItem::insertItem(const wchar *text,int image)
{
	return window->insertItem(this,text,image);
}

void TreeItem::selectItem()
{
	window->selectItem(this);
}

RECT *TreeItem::getItemRect()
{
	return window->getItemRect(this);
}

bool TreeItem::getItemRect(RECT *rc)
{
	return window->getItemRect(this,rc);
}

bool TreeItem::setItemText(const wchar *_text)
{
	bool ret=window->setItemText(this,text);
	if(ret){
		text_is_valid=true;
		free(text);
		text=wcsdup(text);
	}else{
		text_is_valid=false;
		free(text);
		text=NULL;
	}
	return ret;
}

bool TreeItem::getItemText(wchar *buf,int nbuf)
{
	if(text_is_valid){
		wcssafecpy(buf,text,nbuf);
		return true;
	}else{
		return window->getItemText(this,buf,nbuf);
	}
}

const wchar *TreeItem::getItemText()
{
	if(text_is_valid){
		return text;
	}else{
		return window->getItemText(this);
	}
}


bool TreeItem::setItemParam(LPARAM _lParam)
{
	lParam=_lParam;
	lParam_is_valid=true;
	return window->setItemParam(this,lParam);
}

LPARAM TreeItem::getItemParam()
{
	if(lParam_is_valid){
		return lParam;
	}else{
		return window->getItemParam(this);
	}
}
TreeItem TreeItem::getItemNext()
{
	return window->getItemNext(this);
}

TreeItem TreeItem::getItemLast()
{
	return window->getItemLast(this);
}


TreeItem TreeItem::getItemTopChild()
{
	return window->getItemTopChild(this);
}
TreeItem TreeItem::getItemBottomChild()
{
	return window->getItemBottomChild(this);
}

bool TreeItem::setItemChildren(int cChildren)
{
	return window->setItemChildren(this,cChildren);
}
bool TreeItem::beginItemLabelEdit()
{
	return window->beginItemLabelEdit(this);
}

bool TreeItem::endItemLabelEdit(bool cancel)
{
	return window->endItemLabelEdit(cancel);
}

bool TreeItem::deleteItem()
{
	return window->deleteItem(this);
}

//親子関係なしに検索
TreeItem TreeItem::getItemNextByParam(LPARAM lParam)
{
	return window->getItemNextByParam(this,lParam);
}
TreeItem TreeItem::getItemLastByParam(LPARAM lParam)
{
	return window->getItemLastByParam(this,lParam);
}

TreeItem TreeItem::getItemParent()
{
	return window->getItemParent(this);
}

void TreeItem::expandItem()
{
	window->expandItem(this);
}

