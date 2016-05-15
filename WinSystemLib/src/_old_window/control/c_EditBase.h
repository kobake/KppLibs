class EditBase;

#ifndef _EditBase_
#define _EditBase_

#include "c_Window.h"

#define EDITF_KIND(option) (option &	0x0003)		// 0000 0000 0000 0011
#define EDITF_KIND_FIELD				0x0000		// 0000 0000 0000 0000
#define EDITF_KIND_BOX					0x0001		// 0000 0000 0000 0001

#define EDITF_FLAG(option) (option &	0x000C)		// 0000 0000 0000 1100
#define EDITF_FLAG_READONLY				0x0004		// 0000 0000 0000 0100
#define EDITF_FLAG_SELECTONFOCUS		0x0008		// 0000 0000 0000 1000

#define EN_ENTER 1

class EditBase : public Window{
protected:
	bool ctrl_on;
	int edit_kind;
	bool select_on_focus;
public:
	//コンストラクタ・デストラクタ
	void _init_v();
	void _refresh_v();
	EditBase(int x,int y,int w,int h,Window *_parent,int _option=0,int _id=UNIQUE_ID);
	virtual ~EditBase();
	//メッセージ処理
	virtual LRESULT onMessage(UINT msg,WPARAM wParam,LPARAM lParam,BOOL *processed);
	//操作
	void addText(const wstring& text,bool can_undo=true){ addText(text.c_str(),can_undo); }
	void addText(const wchar *text,bool can_undo=true){ setSelEnd(); replaceSel(text,can_undo); }
	void replaceSel(const wchar *text,bool can_undo=true);
	void setSel(int begin,int end);
	void setSelEnd();
	void setSelAll();
	int getSelStart();
	int getSelEnd();
	bool findTextNext(const wchar *word);
	bool findTextLast(const wchar *word);
};





#endif
