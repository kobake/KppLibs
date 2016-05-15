#pragma once

#include "c_Window.h"
#include "c_ImageList.h"

#define TREEVIEWF_EDITLABELS	0x0001

class TreeItem;

class TreeView : public Window, public ImageList{
protected:
	HWND hwndTree;
	WNDPROC treeproc_org;
	bool use_image;
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	TreeView(int x,int y,int w,int h,Window *_parent,int _option=0,int _id=UNIQUE_ID);
	virtual ~TreeView();
	//�ݒ�
	void useImage(bool _use_image=true);
	//�A�C�e���Ǘ�
	TreeItem	insertItem(TreeItem *parent,const wchar *text,int image=-1);
	void		selectItem(TreeItem *item);
	TreeItem	getSelection();
	int			getItemNum();
	TreeItem	getRootItem();
	TreeItem	getTopItem();
	TreeItem	getBottomItem();
	bool		deleteItem(TreeItem *item);
	bool		deleteAllItems();
	TreeItem	getNullItem();
	//�A�C�e������
	bool		getItemRect(TreeItem *item,RECT *rc);
	RECT*		getItemRect(TreeItem *item);
	bool		setItemText(TreeItem *item,const wchar *text);
	bool		getItemText(TreeItem *item,wchar *buf,int nbuf);
	const wchar*	getItemText(TreeItem *item);
	bool		setItemParam(TreeItem *item,LPARAM lParam);
	LPARAM		getItemParam(TreeItem *item);
	TreeItem	getItemTopChild(TreeItem *item);
	TreeItem	getItemBottomChild(TreeItem *item);
	TreeItem	getItemNext(TreeItem *item);
	TreeItem	getItemLast(TreeItem *item);
	TreeItem	getItemNextByParam(TreeItem *item,LPARAM lParam); //�e�q�֌W�Ȃ��Ɍ���
	TreeItem	getItemLastByParam(TreeItem *item,LPARAM lParam); //�e�q�֌W�Ȃ��Ɍ���
	TreeItem	getItemParent(TreeItem *item);
	bool		setItemChildren(TreeItem *item,int cChildren);
	bool		beginItemLabelEdit(TreeItem *item);
	bool		endItemLabelEdit(bool cancel=false);
	void		expandItem(TreeItem *item);
	//���b�Z�[�W����
	LRESULT onNotify(UINT msg,WPARAM wParam,LPARAM lParam);
	LRESULT onSize(UINT msg,WPARAM wParam,LPARAM lParam);
	WNDPROC _getTreeProcOrg(){ return treeproc_org; }
};

#include "c_TreeItem.h"


