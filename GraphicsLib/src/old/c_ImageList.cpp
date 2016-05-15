#include "include_sys.h"
#include "c_ImageList.h"
#include "c_App.h"

ImageList::ImageList(int cx,int cy,int init_size,int max_size)
{
	hImageList=ImageList_Create(cx,cy,ILC_COLOR8 | ILC_MASK,init_size,max_size);
}

ImageList::~ImageList()
{
	ImageList_Destroy(hImageList);
}

int ImageList::addIcon(App *app,int resourceid)
{
	return ImageList_AddIcon(hImageList,LoadIcon(app->getInstance(),MAKEINTRESOURCE(resourceid)));
}

int ImageList::addIcon(Icon *icon)
{
	return ImageList_AddIcon(hImageList,icon->_getHICON());
}

ImageList::operator HIMAGELIST() const
{
	return hImageList;
}

