#include <BaseLib.h>
#include <windows.h>
#include <string>
#include "../../_old_app/c_App.h"
#include "c_EditField.h"
#include <StringLib.h>
#include <DebugLib.h>
using namespace std;



EditField::EditField(int x,int y,int w,int h,Window *_parent,int _option,int _id)
: EditBase(x,y,w,h,_parent,_option | EDITF_KIND_FIELD | WINDOWF_TABSTOP,_id)
{
}

EditField::~EditField()
{
}

