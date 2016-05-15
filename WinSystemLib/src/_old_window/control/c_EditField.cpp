#include <windows.h>
#include <string>
using namespace std;
#include "c_App.h"
#include "c_EditField.h"
#include "include_c_message.h"
#include <StringLib.h>

#include "f_debug.h"



EditField::EditField(int x,int y,int w,int h,Window *_parent,int _option,int _id)
: EditBase(x,y,w,h,_parent,_option | EDITF_KIND_FIELD | WINDOWF_TABSTOP,_id)
{
}

EditField::~EditField()
{
}

