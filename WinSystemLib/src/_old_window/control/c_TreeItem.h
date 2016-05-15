class TreeItem;

#ifndef _TreeItem_
#define _TreeItem_

#define NEW_ITEM	0
#define OLD_ITEM	1

#define _TV_BUFTEXT_MAX 256

class TreeView;

class TreeItem{
protected:
	//������ɂ���Ă����̃p�����[�^���ω����邱�Ƃ͂Ȃ�
	TreeView *window;
	HTREEITEM hItem;
	LPARAM lParam;
	bool lParam_is_valid;
	wchar *text;
	bool text_is_valid;
public:
	friend class TreeView;
	//�R���X�g���N�^�E�f�X�g���N�^
	void _init_v(TreeView *_window,HTREEITEM _hItem,LPARAM _lParam=0,bool _lParam_is_valid=false,const wchar *_text=NULL,bool _text_is_valid=false);
	TreeItem(NMTREEVIEW *nm,int new_or_old);
	TreeItem(NMTVDISPINFO *di);
	TreeItem(TreeView *_window,HTREEITEM _hItem);
	TreeItem(TreeView *_window,HTREEITEM _hItem,LPARAM _lParam);
	TreeItem(TreeItem &right);
	TreeItem();
	virtual ~TreeItem();
	//���
	bool isNull(){ return hItem==NULL; }
	//����
	TreeItem insertItem(const wchar *text,int image=-1);
	void selectItem();
	RECT *getItemRect();
	bool		getItemRect(RECT *rc);
	bool		setItemText(const wchar *_text);
	bool		getItemText(wchar *buf,int nbuf);
	const wchar*	getItemText();
	bool		setItemParam(LPARAM lParam);
	LPARAM		getItemParam();
	TreeItem	getItemNextByParam(LPARAM lParam); //�e�q�֌W�Ȃ��Ɍ���
	TreeItem	getItemLastByParam(LPARAM lParam); //�e�q�֌W�Ȃ��Ɍ���
	TreeItem	getItemParent();
	//
	TreeItem	getItemNext();
	TreeItem	getItemLast();
	TreeItem	getItemTopChild();
	TreeItem	getItemBottomChild();
	bool		setItemChildren(int cChildren);
	bool		beginItemLabelEdit();
	bool		endItemLabelEdit(bool cancel=false);
	bool		deleteItem();
	void		expandItem();

};

#include "c_TreeView.h"

#endif
