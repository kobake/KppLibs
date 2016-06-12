#include <BaseLib.h>
#include <windows.h>
#include <cstring>
#include <string>
#include "../../_old_app/c_App.h"
#include "c_EditBox.h"
#include <StringLib.h>
using namespace std;





EditBox::EditBox(int x,int y,int w,int h,Window *_parent,int _option,int _id)
: EditBase(x,y,w,h,_parent,_option | EDITF_KIND_BOX | WINDOWF_TABSTOP,_id)
{
}


EditBox::~EditBox()
{
}



