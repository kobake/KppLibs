#include <windows.h>
#include <commctrl.h>
#include <string>
using namespace std;
#include "../../_old_app/c_App.h"
#include "c_ListView.h"
#include "../f_window.h"
#include <BaseLib.h>
#include <StringLib.h>
using namespace util;

LRESULT ListView::onSize(UINT msg,WPARAM wParam,LPARAM lParam)
{
	Window::onSize(msg,wParam,lParam);
	MoveWindow(hwndList,0,0,rcClient.right,rcClient.bottom,TRUE);
	return 0L;
}

LRESULT ListView::onFocus(UINT msg,WPARAM wParam,LPARAM lParam)
{
	if(msg==WM_SETFOCUS){
		SetFocus(hwndList);
		return 0L;
	}
	return Window::onActivate(msg,wParam,lParam);
}

ListView::ListView(int x,int y,int w,int h,Window *_parent,int _option,int _id)
: Window(CID_LIST_VIEW,0,_parent,_option | WINDOWF_TABSTOP,_id)
{
	convertXYWH(&x,&y,&w,&h);
	hwnd=CreateAWindow(app->getInstance(),app->getPrevInstance(),L"baseListView",DefWindowProc,(HBRUSH)(COLOR_BTNFACE+1),NULL,NULL,
		0,WS_VISIBLE | WS_CHILD,L"",x,y,w,h,getParent()->getHWND(),(HMENU)getID());
	_afterCreate();
	hwndList=CreateWindowEx(WS_EX_CLIENTEDGE,WC_LISTVIEW,L"",
		WS_VISIBLE | WS_CHILD | LVS_REPORT | WS_MAXIMIZE | LVS_SHOWSELALWAYS,0,0,210,210,hwnd,(HMENU)getID(),app->getInstance(),NULL);
//	hwnd=hwndList=CreateWindowEx(WS_EX_CLIENTEDGE,WC_LISTVIEW,L"",
//		WS_VISIBLE | WS_CHILD | LVS_REPORT | WS_MAXIMIZE | LVS_SHOWSELALWAYS,0,0,0,0,parent->getHWND(),(HMENU)id,app->getInstance(),NULL);
//	_afterCreate();
	messageNotify(true);
}

ListView::~ListView()
{
	DestroyWindow(hwndList);
}



void ListView::insertColumn(int index,const wchar *caption,int width,int align,bool imagecolumn)
{
	LVCOLUMN lvcol;
	lvcol.mask=LVCF_FMT | LVCF_TEXT;
	if(width!=-1)lvcol.mask|=LVCF_WIDTH;
	if(align==-1){
		lvcol.fmt=LVCFMT_LEFT;
	}else if(align==0){
		lvcol.fmt=LVCFMT_CENTER;
	}else if(align==1){
		lvcol.fmt=LVCFMT_RIGHT;
	}
	if(imagecolumn)
		lvcol.fmt|=LVCFMT_IMAGE;
	lvcol.cx=width;		//if -1, ignore
	lvcol.pszText=(wchar*)caption;
	lvcol.cchTextMax=0;	//ignore
	lvcol.iSubItem=0;	//ignore
	lvcol.iImage=0;		//ignore
	lvcol.iOrder=0;		//ignore
	ListView_InsertColumn(hwndList,index,&lvcol);
}

bool ListView_ColumnHasImage(HWND hwnd,int column)
{
	LVCOLUMN col;
	col.mask=LVCF_FMT;
	ListView_GetColumn(hwnd,column,&col);
	return (col.fmt & LVCFMT_IMAGE)!=0;
}

int ListView::insertItem(int index,int subindex,wchar *text,bool setimage,int imageindex,LPARAM lParam)
{
	LVITEM item;
	item.mask=LVIF_TEXT | LVIF_PARAM;
	if(setimage)item.mask|=LVIF_IMAGE;
	item.iItem=index;
	item.iSubItem=subindex;
	item.state=0;		//ignore
	item.stateMask=0;	//ignore
	item.pszText=text;
	item.cchTextMax=0;	//ignore
	item.iImage=imageindex;	//ignore if ==-1
	item.lParam=lParam;
	return ListView_InsertItem(hwndList,&item);
}

int ListView::addItem(int subindex,wchar *text,bool setimage,int imageindex,LPARAM lParam)
{
	return insertItem(ListView_GetItemCount(hwndList),subindex,text,setimage,imageindex,lParam);
}

void ListView::setItem(int index,int subindex,wchar *text,bool setimage,int imageindex)
{
	LVITEM item;
	item.mask=LVIF_TEXT;
	if(setimage)item.mask|=LVIF_IMAGE;
	item.iItem=index;
	item.iSubItem=subindex;
	item.state=0;		//ignore
	item.stateMask=0;	//ignore
	item.pszText=text;
	item.cchTextMax=0;	//ignore
	item.iImage=imageindex;	//ignore if ==-1
	item.lParam=0;		//ignore
	ListView_SetItem(hwndList,&item);
}

void ListView::deleteItem(int index)
{
	ListView_DeleteItem(hwndList,index);
}

void ListView::clear()
{
	ListView_DeleteAllItems(hwndList);
}

void ListView::setImageList(HIMAGELIST hImageList)
{
	ListView_SetImageList(hwndList,hImageList,LVSIL_SMALL); //###LVSIL_STATEのほうがいい？？
}


struct CompInfo{
	HWND hwndList;
	int column;
	bool az;
};

static int CALLBACK CompProcString(LPARAM lParam1,LPARAM lParam2,LPARAM lParamSort)
{
	static wchar buf1[256],buf2[256];
	CompInfo *info=(CompInfo*)lParamSort;
	//アイテムの検索
	LVFINDINFO fi; int n1,n2;
	fi.flags=LVFI_PARAM;
	fi.lParam=lParam1;
	n1=ListView_FindItem(info->hwndList,-1,&fi);
	fi.lParam=lParam2;
	n2=ListView_FindItem(info->hwndList,-1,&fi);
	//テキストの取得
	ListView_GetItemText(info->hwndList,n1,info->column,buf1,256);
	ListView_GetItemText(info->hwndList,n2,info->column,buf2,256);
	//比較
	if(info->az){
		return wcsicmp(buf1,buf2);
	}else{
		return wcsicmp(buf2,buf1);
	}
}

void ListView::sortItemsByFunc(PFNLVCOMPARE pfnCompare,LPARAM lParamSort)
{
	ListView_SortItems(hwndList,pfnCompare,lParamSort);
}

void ListView::sortItemsByString(int column,bool az)
{
	static CompInfo info;
	info.hwndList=hwndList;
	info.column=column;
	info.az=az;
	ListView_SortItems(hwndList,CompProcString,(LPARAM)&info);
}

static int CALLBACK CompProclParam(LPARAM lParam1,LPARAM lParam2,LPARAM lParamSort)
{
	//比較
	if(lParamSort){ //AZ
		return lParam1-lParam2;
	}else{ //ZA
		return lParam2-lParam1;
	}
}

void ListView::sortItemsBylParam(bool az)
{
	ListView_SortItems(hwndList,CompProclParam,(LPARAM)az?1:0);
}


int ListView::getSelectedIndex()
{
	return ListView_GetNextItem(hwndList,-1,LVNI_SELECTED);
}

int ListView::getSelectedParam()
{
	LVITEM item;
	item.iItem=getSelectedIndex();
	item.iSubItem=0;
	item.mask=LVIF_PARAM;
	item.lParam=-1;
	ListView_GetItem(hwndList,&item);
	return item.lParam;
}

ChainList<int> *ListView::getSelectedParams()
{
	static ChainList<int> list;
	LVITEM item;
	item.iItem=-1; item.iSubItem=0;
	item.mask=LVIF_PARAM; item.lParam=-1;
	list.clear();
	while((item.iItem=ListView_GetNextItem(hwndList,item.iItem,LVNI_SELECTED))!=-1){
		ListView_GetItem(hwndList,&item);
		list.put(-1,item.lParam);
	}
	return &list;
}


wchar *ListView::getSelectedText(int iSubItem)
{
	static wchar buf[256];
	LVITEM item;
	item.iItem=getSelectedIndex();
	item.iSubItem=iSubItem;
	item.mask=LVIF_TEXT;
	item.pszText=buf;
	item.cchTextMax=sizeof(buf);
	ListView_GetItem(hwndList,&item);
	return item.pszText;
}


void ListView::removeFoundByText(wchar *text,int iSubItem)
{
	int i,n=ListView_GetItemCount(hwndList);
	int nbuf=wcslen(text)+1;
	wchar *buf=(wchar*)calloc(nbuf,sizeof(wchar)); buf[nbuf-1]=L'\0';
	for(i=0;i<n;i++){
		ListView_GetItemText(hwndList,i,iSubItem,buf,nbuf);
		if(wcscmp(buf,text)==0){
			ListView_DeleteItem(hwndList,i);
			n--;
			i--;
		}
	}
}
