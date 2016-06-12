#pragma once

#include <Windows.h>
#include <CommCtrl.h>

class Icon;
class App;

class ImageList{
private:
	HIMAGELIST hImageList;
public:
	ImageList(int cx,int cy,int init_size,int max_size);
	virtual ~ImageList();
	int addIcon(App *app,int resourceid);
	int addIcon(Icon *icon);
public:
	operator HIMAGELIST() const;
};
