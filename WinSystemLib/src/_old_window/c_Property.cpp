#include "include_sys.h"
#include "c_Property.h"
#include "c_CustomWindow.h"
#include "c_Label.h"
#include "c_Button.h"
#include "c_Screen.h"
#include <StringLib.h>
#include "c_EditField.h"
#include "c_ComboBox.h"
#include "id/class.h"
#include "c_App.h"
using namespace util;

static PropertyRList *property_list,*property_list_org;
static int label_w;
static Font font(L"ＭＳ ゴシック",9);
static bool active;
static bool dlg_ret;
static CHECKPROC dlg_checkproc;

// ** ** ** ** PropertyDialog ** ** ** ** //

class PropertyDialogWindow : public CustomWindow{
protected:
	int line_y[256];
	Label *lblName[256];
	Window *winValue[256];
	Button *btnOk,*btnCancel;
public:
	void EndPropertyDialog(bool _ret)
	{
		dlg_ret=_ret;
		if(getParent()!=NULL){
			getParent()->toFront(); //BringWindowToTop(hwndOwner);
			getParent()->setEnable(true);
		}
		this->destroy();
	}
public:
	PropertyDialogWindow(const wchar *caption,int x,int y,int w,int h,Window *_parent,int _id=-2,int _option=0)
	: CustomWindow(caption,x,y,w,h,_parent,_option,_id)
	{
		int i;
		Window::setFontOnCreate(font);
		//入力シート作成
		for(i=0;i<property_list->size();i++){
			lblName[i]=new Label(property_list->get(i)->getPropertyName(),0,0,0,0,this);
			winValue[i]=property_list->get(i)->createWindow(0,0,0,0,this);
		}
		//OK,Cancelボタン作成
		btnOk=new Button(L"OK",0,0,0,0,this,IDOK,BUTTONF_KIND_DEFAULTBUTTON);
		btnCancel=new Button(L"キャンセル",0,0,0,0,this,IDCANCEL);
		//初期値設定
		for(i=0;i<property_list->size();i++){
			property_list->get(i)->setToWindow(winValue[i]);
		}
		//初期フォーカス
		winValue[0]->setFocus();
		if(winValue[0]->getType()==CID_EDIT_BASE){
			((EditBase*)(winValue[0]))->setSelAll();
		}
		messageNotify(true);
	}
	LRESULT onSize(UINT msg,WPARAM wParma,LPARAM lParam)
	{
		int i,x,y,w,h,ww,hh;
		//
		ww=LOWORD(lParam);
		hh=HIWORD(lParam);
		//
		w=80;
		h=22;
		y=4;
		ww-=4+w; x=ww;
		btnOk->move(x,y,w,h);
		y+=h+4;
		btnCancel->move(x,y,w,h);
		//
		y=4;
		for(i=0;i<property_list->size();i++){
			x=4;
			w=label_w;
			h=22;
			lblName[i]->move(x,y+2,w,h);
			//
			x+=w+4;
			w=ww-(4+w+4+4);
			h=property_list->get(i)->getControlH();
			winValue[i]->move(x,y,w,h);
			//
			y+=h+2;
		}
		return 0L;
	}
	LRESULT onCommand(UINT msg,WPARAM wParam,LPARAM lParam)
	{
		int i;
		if(HIWORD(wParam)==0)switch(LOWORD(wParam)){
		case IDOK:
			//入力値の検証
			for(i=0;i<property_list->size();i++){
				if(!property_list->get(i)->isValidWindowValue(winValue[i])){
					property_list->get(i)->messageRule(this);
					return 0L;
				}
			}
			//入力値を取得
			for(i=0;i<property_list->size();i++){
				property_list->get(i)->getFromWindow(winValue[i]);
			}
			//さらに検証
			if(dlg_checkproc!=NULL){
				const wchar *ret=dlg_checkproc(property_list);
				if(ret!=NULL){
					messageBox(ret,L"注意",MB_OK);
					return 0L;
				}
			}
			//ダイアログを閉じる
			EndPropertyDialog(true);
			break;
		case IDCANCEL:
			EndPropertyDialog(false);
			break;
		}
		return 0L;
	}
	LRESULT onClose(UINT msg,WPARAM wParam,LPARAM lParam)
	{
		EndPropertyDialog(false);
		return 0L;
	}
	LRESULT onDestroy(UINT msg,WPARAM wParam,LPARAM lParam)
	{
		active=false;
		return 0L;
	}
};

bool PropertyDialog(const wchar *caption,int x,int y,int w,int h,Window *owner,CHECKPROC _checkproc,PropertyRList *_property_list)
{
	int n,i;
	dlg_checkproc=_checkproc;
	if(_property_list->size()==0)return false;
	//リスト準備
	property_list_org=_property_list;
	property_list=new PropertyRList();
	for(i=0;i<property_list_org->size();i++){
		property_list->put(-1,property_list_org->get(i)->clone());
	}
	//ラベル幅計算
	for(i=0;i<property_list->size();i++){
		n=font.getTextWidth(property_list->get(i)->getPropertyName());
		label_w=tmax<int>(label_w,n);
	}
	//ダイアログ位置決定
	if(w==-1)
		w=label_w+80+170;
	if(h==-1){
		int yy=4;
		for(i=0;i<property_list->size();i++){
			yy+=property_list->get(i)->getControlH()+2;
		}
		h=tmax<int>( (4+22+4+22+4)+24, (yy+4)+24 );
	}
	if(x==-1)
		x=(screen->getWidth()-w)/2;
	if(y==-1)
		y=(screen->getHeight()-h)/2;
	//ダイアログ表示
	active=true;
	dlg_ret=false;
	Window *winProperty=new PropertyDialogWindow(caption,x,y,w,h,owner);
	winProperty->show(SW_SHOW);
	//
	if(owner!=NULL)owner->setEnable(false);
	//メッセージループ
	//app->windowsLoop();
	MSG msg; HWND hwnd=winProperty->getHWND();
	while(active){ //### できれば active変数を使わずに処理したい
		if(!GetMessage(&msg,NULL,0,0))break;
		if(IsDialogMessage(hwnd,&msg)==FALSE){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	//リスト後始末
	for(i=0;i<property_list->size();i++){
		property_list->get(i)->copyDataTo(property_list_org->get(i));
		delete property_list->get(i);
	}
	delete property_list;
	//戻る
	return dlg_ret;
}

bool PropertyDialog(const wchar *caption,int x,int y,int w,int h,Window *owner,CHECKPROC _checkproc,Property *_property,...)
{
	PropertyRList *_property_list=new PropertyRList();
	//引数リストの取得
	va_list mark;
	va_start(mark,_property);
	Property *p=_property;
	while(p!=NULL){
		_property_list->put(-1,p);
		p=va_arg(mark,Property*);
		if(_property_list->size()>=256)break;
	}
	va_end(mark);
	//実行
	bool ret=PropertyDialog(caption,x,y,w,h,owner,_checkproc,_property_list);
	//後始末
	delete _property_list;
	//
	return ret;
}

// ** ** ** ** Property ** ** ** ** //

Property::Property(int _property_type,wchar *_property_name)
{
	property_type=_property_type;
	wcssafecpy(property_name,_property_name,sizeof(property_name));
}

Property::~Property()
{
}

wchar *Property::getPropertyName()
{
	return property_name;
}

void Property::setPropertyName(wchar *_property_name)
{
	wcssafecpy(property_name,_property_name,sizeof(property_name));
}


// ** ** ** ** IntProperty ** ** ** ** //

Window *IntProperty::createWindow(int x,int y,int w,int h,Window *_parent)
{
	return new EditField(x,y,w,h,_parent);
}

void IntProperty::setToWindow(Window *wnd)
{
	wnd->setText(tmp_itow(data));
}

bool IntProperty::getFromWindow(Window *wnd)
{
	set(_wtoi(wnd->getTextTemp()));
	return true;
}

bool IntProperty::isValidWindowValue(Window *wnd)
{
	return isRange(_wtoi(wnd->getTextTemp()));
}

void IntProperty::messageRule(Window *owner)
{
	owner->messageBoxF(L"注意",MB_OK,L"%sの値が無効です\n%d～%dの範囲の整数を入力してください",getPropertyName(),data_min,data_max);
}

IntProperty::IntProperty(wchar *_property_name,int _data_min,int _data_max)
: Property(INT_PROPERTY,_property_name)
{
	setRange(_data_min,_data_max);
	data=0;
}
IntProperty::~IntProperty()
{
}
int IntProperty::get()
{
	return data;
}
void IntProperty::set(int _data)
{
	data=_data;
	if(data<data_min)data=data_min;
	else if(data>data_max)data=data_max;
}

void IntProperty::setRange(int _data_min,int _data_max)
{
	data_min=_data_min;
	data_max=_data_max;
	if(data_max<data_min)data_max=data_min;
	set(data);
}

bool IntProperty::isRange(int _data)
{
	return (_data>=data_min && _data<=data_max);
}

int IntProperty::write(FILE *fp)
{
	fwrite(&data,sizeof(int),1,fp);
	return 0;
}

int IntProperty::read(FILE *fp)
{
	fread(&data,sizeof(int),1,fp);
	return 0;
}


// ** ** ** ** CharsProperty ** ** ** ** //

Window *CharsProperty::createWindow(int x,int y,int w,int h,Window *_parent)
{
	return new EditField(x,y,w,h,_parent);
}

void CharsProperty::setToWindow(Window *wnd)
{
	wnd->setText(data);
}

bool CharsProperty::getFromWindow(Window *wnd)
{
	set(wnd->getTextTemp());
	return true;
}

bool CharsProperty::isValidWindowValue(Window *wnd)
{
	int n=wcslen(wnd->getTextTemp());
	return (n<size-1 && n>=size_min);
}

void CharsProperty::messageRule(Window *owner)
{
	owner->messageBoxF(L"注意",MB_OK,L"%sの値が無効です\n半角%d文字以上%d文字以下で入力してください",getPropertyName(),size_min,size-1);
}

CharsProperty::CharsProperty(wchar *_property_name,int _size,int _size_min)
: Property(CHARS_PROPERTY,_property_name)
{
	size_min=_size_min;
	size=_size;
	data=(wchar*)calloc(size,sizeof(wchar));
	*data=L'\0';
}
CharsProperty::~CharsProperty()
{
	free(data);
}
wchar *CharsProperty::get()
{
	return data;
}
void CharsProperty::set(const wstring& _data)
{
	wcssafecpy(data,_data.c_str(),size);
}

int CharsProperty::write(FILE *fp)
{
	fwrite(data,sizeof(wchar),size,fp);
	return 0;
}

int CharsProperty::read(FILE *fp)
{
	fread(data,sizeof(wchar),size,fp);
	data[size-1]=L'\0';
	return 0;
}


// ** ** ** ** EnumProperty ** ** ** ** //

Window *EnumProperty::createWindow(int x,int y,int w,int h,Window *_parent)
{
	ComboBox *ret=new ComboBox(x,y,w,h,_parent);
	for(int i=0;i<(int)elementlist->size();i++){
		ret->addString(elementlist->at(i).c_str());
	}
	return (Window*)ret;
}

void EnumProperty::setToWindow(Window *_wnd)
{
	ComboBox *wnd=(ComboBox*)_wnd;
	wnd->setSelected(data);
}

bool EnumProperty::getFromWindow(Window *_wnd)
{
	ComboBox *wnd=(ComboBox*)_wnd;
	data=wnd->getSelected();
	return true;
}

bool EnumProperty::isValidWindowValue(Window *_wnd)
{
	ComboBox *wnd=(ComboBox*)_wnd;
	if(wnd->getSelected()==-1){
		return false;
	}else{
		return true;
	}
}

void EnumProperty::messageRule(Window *owner)
{
	owner->messageBoxF(L"注意",MB_OK,L"%sの値を選択してください",getPropertyName());
}

int EnumProperty::write(FILE *fp)
{
	fwrite(&data,sizeof(int),1,fp);
	return 0;
}

int EnumProperty::read(FILE *fp)
{
	fread(&data,sizeof(int),1,fp);	
	return 0;
}

EnumProperty::EnumProperty(wchar *_property_name,StringList *_elementlist)
: Property(ENUM_PROPERTY,_property_name)
{
	elementlist=_elementlist->clone();
	data=0;
}

EnumProperty::EnumProperty(wchar *_property_name,wchar *element0,...)
: Property(ENUM_PROPERTY,_property_name)
{
	elementlist=new StringList();
	va_list mark;
	va_start(mark,element0);
	wchar *p=element0;
	while(p!=NULL){
		elementlist->put(-1,p);
		p=va_arg(mark,wchar*);
	}
	va_end(mark);
	data=0;
}

EnumProperty::~EnumProperty()
{
}

int EnumProperty::get()
{
	return data;
}

void EnumProperty::set(int _selected)
{
	data=_selected;
}

