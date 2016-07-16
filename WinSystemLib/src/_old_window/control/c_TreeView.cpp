#include <windows.h>
#include <commctrl.h>
#include <string>
#include <BaseLib.h>
#include "c_TreeView.h"
#include "../../_old_app/c_App.h"
#include "../f_window.h"
using namespace std;

LRESULT CALLBACK WndProcTreeView(HWND hwndTree,UINT msg,WPARAM wParam,LPARAM lParam);


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- --  コンストラクタ・デストラクタ  -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
TreeView::TreeView(int x,int y,int w,int h,Window *_parent,int _option,int _id)
: Window(CID_TREEVIEW,0,_parent,_id)
, ImageList(16,16,0,32)
{
	convertXYWH(&x,&y,&w,&h);
	use_image=false;
	//
	InitCommonControls();
	//ツリーオーナー作成
	hwnd=CreateWindowEx(0,L"STATIC",L"",
		WS_CHILD | WS_VISIBLE,
		x,y,w,h,getParent()->getHWND(),(HMENU)getID(),g_app->getInstance(),NULL);
	_afterCreate();
	//スタイル
	DWORD style=WS_CHILD | WS_VISIBLE | WS_MAXIMIZE |
		TVS_HASLINES | TVS_HASBUTTONS | TVS_LINESATROOT | TVS_SHOWSELALWAYS;
	if(_option & TREEVIEWF_EDITLABELS){
		style|=TVS_EDITLABELS;
	}
	//ツリー作成
	hwndTree=CreateWindowEx(WS_EX_CLIENTEDGE,WC_TREEVIEW,L"",
		style,0,0,100,100,hwnd,(HMENU)getID(),g_app->getInstance(),NULL);
	SetWindowLong(hwndTree,GWL_USERDATA,(LONG)this);
	treeproc_org=(WNDPROC)SetWindowLong(hwndTree,GWL_WNDPROC,(LONG)WndProcTreeView);
	//
	messageNotify(true);
}
TreeView::~TreeView()
{
	SetWindowLong(hwndTree,GWL_WNDPROC,(LONG)treeproc_org);
	DestroyWindow(hwndTree);
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- --   設定   -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
void TreeView::useImage(bool _use_image)
{
	if(use_image==_use_image)return;
	use_image=_use_image;
	if(use_image){
		TreeView_SetImageList(hwndTree,operator HIMAGELIST(),TVSIL_NORMAL);
	}else{
		TreeView_SetImageList(hwndTree,NULL,TVSIL_NORMAL);
	}
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- アイテム管理 -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
#include <DebugLib.h>

TreeItem TreeView::insertItem(TreeItem *parent,const wchar *text,int image)
{
	HTREEITEM hItem;
	TVINSERTSTRUCT is;
	memset(&is,0,sizeof(is));
	is.hParent=((parent!=NULL)?parent->hItem:TVI_ROOT);
	is.hInsertAfter=TVI_LAST;
	is.item.mask=TVIF_TEXT;
	if(image>=0)
		is.item.mask|=TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	is.item.pszText=(wchar*)text;
	is.item.iImage=image;
	is.item.iSelectedImage=image;
	hItem=TreeView_InsertItem(hwndTree,&is);
	//
	if(parent!=NULL){
		RECT rc;
		if(parent->getItemRect(&rc)){
			repaint(&rc,false);
		}
//		parent->setItemChildren(1);
	}
	return TreeItem(this,hItem);
}

void TreeView::selectItem(TreeItem *item)
{
	TreeView_SelectItem(hwndTree,item?item->hItem:NULL);
}

TreeItem TreeView::getSelection()
{
	return TreeItem(this,TreeView_GetSelection(hwndTree));
}

int	TreeView::getItemNum()
{
	return TreeView_GetCount(hwndTree);
}

TreeItem TreeView::getRootItem()
{
	HTREEITEM hItem=TreeView_GetRoot(hwndTree);
	return TreeItem(this,hItem);
}

TreeItem TreeView::getTopItem()
{
	HTREEITEM hItem=TreeView_GetRoot(hwndTree);
	return TreeItem(this,hItem);
}

TreeItem TreeView::getBottomItem()
{
	TreeItem new_item,item=getTopItem();
	if(!item.isNull()){
		//兄弟の一番下を取得
		do{
			new_item=item.getItemNext();
			if(!new_item.isNull()){
				item=new_item;
			}
		}while(!new_item.isNull());
		//兄弟の一番下の子供を取得
		new_item=item.getItemBottomChild();
		if(!new_item.isNull()){
			item=new_item;
		}
	}
	return item;
}


TreeItem TreeView::getItemNext(TreeItem *item)
{
	if(item==NULL || item->isNull()){
		return getRootItem();
	}
	HTREEITEM hItem=TreeView_GetNextSibling(hwndTree,item->hItem);
	return TreeItem(this,hItem);
}

TreeItem TreeView::getItemLast(TreeItem *item)
{
	if(item==NULL || item->isNull()){
		return getBottomItem();
	}
	HTREEITEM hItem=TreeView_GetPrevSibling(hwndTree,item->hItem);
	return TreeItem(this,hItem);
}

bool TreeView::deleteItem(TreeItem *item)
{
	return TreeView_DeleteItem(hwndTree,item->hItem)!=0;
}
bool TreeView::deleteAllItems()
{
	return TreeView_DeleteAllItems(hwndTree)!=0;
}
TreeItem TreeView::getNullItem()
{
	return TreeItem(this,NULL);
}

//親子関係なしに検索
TreeItem TreeView::getItemNextByParam(TreeItem *_item,LPARAM lParam)
{
	TreeItem item;
	TreeItem next_item,next_next_item;
	TreeItem parent_item;
	//
	if(_item!=NULL){
		item=*_item;
	}else{
		item=getNullItem();
	}
	//
	//まず子を検索
	next_item=item.getItemTopChild();
	if(!next_item.isNull()){
		if(next_item.getItemParam()==lParam){
			return next_item;
		}
		next_next_item=next_item.getItemNextByParam(lParam);
		if(!next_next_item.isNull()){
			return next_next_item;
		}
	}
	//兄弟を検索
	next_item=item.getItemNext();
	while(!next_item.isNull()){
		if(!next_item.isNull()){
			if(next_item.getItemParam()==lParam){
				return next_item;
			}
		}
		next_item=next_item.getItemNext();
	}
	//親を検索
	parent_item=item.getItemParent();
	if(!parent_item.isNull()){
		next_item=parent_item.getItemNext();
		if(!next_item.isNull()){
			if(next_item.getItemParam()==lParam){
				return next_item;
			}
			next_item=next_item.getItemNextByParam(lParam);
			if(!next_item.isNull()){
				return next_item;
			}
		}
	}
	return getNullItem();
}

//親子関係なしに検索
TreeItem TreeView::getItemLastByParam(TreeItem *_item,LPARAM lParam)
{
	TreeItem item;
	TreeItem last_item;
	TreeItem parent_item;
	TreeItem child_item;
	//
	if(_item!=NULL){
		item=*_item;
	}else{
		item=getNullItem();
	}
	//
	//まず兄弟(もしくは兄弟の最下子)を検索
	last_item=item.getItemLast();
	if(!last_item.isNull()){
		do{
			//兄弟の子
			child_item=last_item.getItemBottomChild();
			if(!child_item.isNull()){
				if(child_item.getItemParam()==lParam){
					return child_item;
				}
				child_item=getItemLastByParam(&child_item,lParam);
			}
			//兄弟
			if(last_item.getItemParam()==lParam){
				return last_item;
			}
			last_item=last_item.getItemLast();
		}while(!last_item.isNull());
	}
	//親を検索
	parent_item=item.getItemParent();
	if(!parent_item.isNull()){
		//検証
		if(parent_item.getItemParam()==lParam){
			return parent_item;
		}else{
			last_item=parent_item.getItemLastByParam(lParam);
			if(!last_item.isNull()){
				return last_item;
			}
		}
	}
	return getNullItem();
}


TreeItem TreeView::getItemParent(TreeItem *item)
{
	HTREEITEM hItem=TreeView_GetParent(hwndTree,item->hItem);
	return TreeItem(this,hItem);
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- アイテム操作 -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

RECT *TreeView::getItemRect(TreeItem *item)
{
	static RECT rc;
	if(getItemRect(item,&rc)){
		return &rc;
	}else{
		return NULL;
	}
}

bool TreeView::getItemRect(TreeItem *item,RECT *rc)
{
	BOOL ret=TreeView_GetItemRect(hwndTree,item->hItem,rc,FALSE);
	return ret!=FALSE;
}

bool TreeView::setItemText(TreeItem *item,const wchar *text)
{
	TVITEM tvi;
	tvi.hItem=item->hItem;
	tvi.mask=TVIF_HANDLE | TVIF_TEXT;
	tvi.pszText=(wchar*)text;
	tvi.cchTextMax=0; //ignored
	return TreeView_SetItem(hwndTree,&tvi)!=0;
}

bool TreeView::getItemText(TreeItem *item,wchar *buf,int nbuf)
{
	TVITEM tvi;
	tvi.hItem=item->hItem;
	tvi.mask=TVIF_HANDLE | TVIF_TEXT;
	tvi.pszText=buf;
	tvi.cchTextMax=nbuf;
	return TreeView_GetItem(hwndTree,&tvi)!=0;
}

const wchar *TreeView::getItemText(TreeItem *item)
{
	static wchar buf[_TV_BUFTEXT_MAX];
	if(getItemText(item,buf,_TV_BUFTEXT_MAX-1)){
		return buf;
	}else{
		return NULL;
	}
}
bool TreeView::setItemParam(TreeItem *item,LPARAM lParam)
{
	TVITEM tvi;
	tvi.mask=TVIF_HANDLE | TVIF_PARAM;
	tvi.hItem=item->hItem;
	tvi.lParam=lParam;
	return TreeView_SetItem(hwndTree,&tvi)!=0;
}
LPARAM TreeView::getItemParam(TreeItem *item)
{
	TVITEM tvi;
	tvi.mask=TVIF_HANDLE | TVIF_PARAM;
	tvi.hItem=item->hItem;
	BOOL ret=TreeView_GetItem(hwndTree,&tvi);
	if(ret){
		return tvi.lParam;
	}else{
		return 0;
	}
}
TreeItem TreeView::getItemTopChild(TreeItem *item)
{
	HTREEITEM hItem=TreeView_GetChild(hwndTree,item->hItem);
	return TreeItem(this,hItem);
}
TreeItem TreeView::getItemBottomChild(TreeItem *item)
{
	HTREEITEM hItemNew,hItem=TreeView_GetChild(hwndTree,item->hItem);
	if(hItem!=NULL){
		do{
			hItemNew=TreeView_GetNextSibling(hwndTree,hItem);
			if(hItemNew!=NULL){
				hItem=hItemNew;
			}
		}while(hItemNew!=NULL);
	}else{
		hItem=NULL;
	}
	return TreeItem(this,hItem);
}

bool TreeView::setItemChildren(TreeItem *item,int cChildren)
{
	TVITEM tvi;
	tvi.mask=TVIF_HANDLE | TVIF_CHILDREN;
	tvi.cChildren=cChildren;
	return TreeView_SetItem(hwndTree,&tvi)!=0;
}

bool TreeView::beginItemLabelEdit(TreeItem *item)
{
	HWND hwnd=(HWND)SendMessage(hwndTree,TVM_EDITLABEL,0,(LPARAM)item->hItem); //エディットコントロールのハンドルが返る
	return (hwnd!=NULL);
}

bool TreeView::endItemLabelEdit(bool cancel)
{
	return SendMessage(hwndTree,TVM_ENDEDITLABELNOW,cancel?TRUE:FALSE,0)!=0;
}

void TreeView::expandItem(TreeItem *item)
{
	TreeView_Expand(hwndTree,item->hItem,TVE_EXPAND);
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- メッセージ処理  -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

#include <DebugLib.h>

LRESULT CALLBACK WndProcTreeView(HWND hwndTree,UINT msg,WPARAM wParam,LPARAM lParam)
{
	TreeView *tree=(TreeView*)GetWindowLong(hwndTree,GWL_USERDATA);
	if(tree==NULL)
		return DefWindowProc(hwndTree,msg,wParam,lParam);
	if(msg==WM_LBUTTONDOWN || msg==WM_RBUTTONDOWN){
		TVHITTESTINFO ht;
		ht.pt.x=LOWORD(lParam);
		ht.pt.y=HIWORD(lParam);
		ht.flags=0;
		ht.hItem=NULL;
		HTREEITEM hItem=TreeView_HitTest(hwndTree,&ht);
		if(hItem==NULL || (ht.flags & TVHT_ONITEMRIGHT) || (ht.flags & TVHT_ONITEMINDENT)){
			//アイテム外をクリックしたとき
			tree->endItemLabelEdit(false); //編集を解除
			tree->selectItem(NULL); //選択を解除
			return 0L;
		}else{
			return CallWindowProc(tree->_getTreeProcOrg(),hwndTree,msg,wParam,lParam);
		}
	}else{
		return CallWindowProc(tree->_getTreeProcOrg(),hwndTree,msg,wParam,lParam);
	}
	return 0L;
}

LRESULT TreeView::onNotify(UINT msg,WPARAM wParam,LPARAM lParam)
{
	int id=wParam;
	NMHDR* nh=(NMHDR*)lParam;
	if((int)nh->idFrom==getID()){
		nh->hwndFrom=hwnd;
		if(nh->code==TVN_KEYDOWN){
			NMTVKEYDOWN *nmk=(NMTVKEYDOWN*)lParam;
			if(nmk->wVKey==VK_F2){
				TreeItem item=getSelection();
				if(!item.isNull()){
					beginItemLabelEdit(&item);
				}
			}
			return getParent()->sendMessage(msg,wParam,lParam);
		}else{
			return getParent()->sendMessage(msg,wParam,lParam);
		}
	}else{
		return Window::onNotify(msg,wParam,lParam);
	}
}


LRESULT TreeView::onSize(UINT msg,WPARAM wParam,LPARAM lParam)
{
	onClientSize(LOWORD(lParam),HIWORD(lParam));
	MoveWindow(hwndTree,0,0,getClientWidth(),getClientHeight(),TRUE);
	return 0L;
}

