#pragma once

#include "../c_Window.h"
#include <BaseLib.h> // t_Chain

class ListView : public Window{
protected:
	HWND hwndList;
public:
	ListView(int x,int y,int w,int h,Window *_parent,int _option=0,int _id=UNIQUE_ID);
	virtual ~ListView();
	//挿入・削除
	void insertColumn(int index,const wchar *caption,int width,int align,bool imagecolumn);
	int insertItem(int index,int subindex,wchar *text,bool setimage,int imageindex,LPARAM lParam);
	int addItem(int subindex,wchar *text,bool setimage,int imageindex,LPARAM lParam);
	void setItem(int index,int subindex,wchar *text,bool setimage,int imageindex);
	void clear();
	void deleteItem(int index);
	//ソート
	void setImageList(HIMAGELIST hImageList);
	void sortItemsByString(int column,bool az);
	void sortItemsBylParam(bool az);
	void sortItemsByFunc(PFNLVCOMPARE pfnCompare,LPARAM lParamSort);
	//状態取得
	int getSelectedIndex();
	int getSelectedParam();
	wchar *getSelectedText(int iSubItem=0);
	ChainList<int> *getSelectedParams();
	//検索
	void removeFoundByText(wchar *text,int iSubItem=0);
	//
	virtual LRESULT onSize(UINT msg,WPARAM wParam,LPARAM lParam);
	virtual LRESULT onFocus(UINT msg,WPARAM wParam,LPARAM lParam);
};
