#if 0

#pragma once

#include <GraphicsLib.h>
#include "c_ListView.h"
#include "c_FileList.h"
#include "c_FileName.h"
#include "c_Menu.h"

class FileView : public ListView{
protected:
	int IDIL_FOLDER,IDIL_FILE;
	ImageList imagelist;
	FileList filelist;
	FileName file;
// ** ** ** ** ソート管理 ** ** ** ** //
protected:
	int sort_column;
	bool sort_az;
	void sort(int _sort_column,bool _sort_az);
public:
	int compareProc(LPARAM lParam1,LPARAM lParam2);

// ** ** ** ** 基本 ** ** ** ** //
public:
	FileView(int x,int y,int w,int h,Window *_parent,int _option=0,int _id=UNIQUE_ID);
	virtual ~FileView();
	void setDir(const wstring& dir);
	void chDir(const wstring& dir);
	void realizeDir();
	string getDir();
	//
	virtual LRESULT onNotify(UINT msg,WPARAM wParam,LPARAM lParam);
};


#endif
