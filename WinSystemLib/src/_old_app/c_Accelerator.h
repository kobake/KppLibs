class Accelerator;

#ifndef DEF_ACCELERATOR
#define DEF_ACCELERATOR

#include "c_Window.h"


#define ACCELF_CONTROL	1
#define ACCELF_SHIFT	2

struct ACCELDEF{
	int cmd;
	int key;
	int flag;
};

class Accelerator{
protected:
	ACCEL *table;
	int ntable;
	HACCEL hAccel;
	bool changed;
public:
	Accelerator(App *app,wchar* resourcename);
	Accelerator();
	virtual ~Accelerator();
	//ハンドル操作
	void _destroyAccelerator();
	void _updateAccelerator();
	void _copyAccelerator();
	void loadAccelerator(App *app,wchar* resourcename);
	//エントリー編集
	void setEntries(ACCELDEF *acc);
	void clearEntries();
	void addEntry(int cmd,int key,int flag);
	int findEntry(int cmd,int key,int flag);
	void deleteEntry(int index);
	void deleteEntry(int cmd,int key,int flag);
	//実使用
	int translate(Window *wnd,MSG *msg);
	int translate(HWND hwnd,MSG *msg);
	//
protected:
	static BYTE _flag_to_virt(int flag);
};


/*
ACC_MAIN ACCELERATORS DISCARDABLE 
BEGIN
    "O",            IDM_FILE_OPEN,          VIRTKEY, CONTROL, NOINVERT
    "S",            IDM_FILE_SAVE,          VIRTKEY, CONTROL, NOINVERT
    VK_F1,          IDM_FILE_NEW,           VIRTKEY, CONTROL, NOINVERT
    "Z",            IDM_EDIT_UNDO,          VIRTKEY, CONTROL, NOINVERT
END
*/
#endif

