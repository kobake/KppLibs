class Property;
class IntProperty;
class CharsProperty;

#ifndef DEF_PROPERTY
#define DEF_PROPERTY

#include <StringLib.h> // c_StringList
#include "./c_Window.h"
#include "c_PropertyRList.h"

bool PropertyDialog(const wchar *caption,int x,int y,int w,int h,Window *owner,CHECKPROC _checkproc,Property *_property,...);
bool PropertyDialog(const wchar *caption,int x,int y,int w,int h,Window *owner,CHECKPROC _checkproc,PropertyRList *_property_list);

class Property{
public:
	virtual Window *createWindow(int x,int y,int w,int h,Window *_parent)=0;
	virtual int getControlH()=0;
	virtual void setToWindow(Window *wnd)=0;
	virtual bool getFromWindow(Window *wnd)=0;
	virtual bool isValidWindowValue(Window *wnd)=0;
	virtual void messageRule(Window *owner)=0;
public:
	virtual int write(FILE *fp)=0;
	virtual int read(FILE *fp)=0;
	virtual void copyDataTo(Property *_property)=0;
	virtual Property *clone()=0;
protected:
	int property_type;
	wchar property_name[32];
public:
	Property(int _property_type,wchar *_property_name);
	virtual ~Property();
public:
	wchar *getPropertyName();
	void setPropertyName(wchar *_property_name);
};


// ** ** ** ** IntProperty ** ** ** ** //

#define INT_PROPERTY 0

class IntProperty : public Property{
public:
	virtual Window *createWindow(int x,int y,int w,int h,Window *_parent);
	virtual int getControlH(){ return 22; }
	virtual void setToWindow(Window *wnd);
	virtual bool getFromWindow(Window *wnd);
	virtual bool isValidWindowValue(Window *wnd);
	virtual void messageRule(Window *owner);
public:
	virtual int write(FILE *fp);
	virtual int read(FILE *fp);
	virtual void copyDataTo(Property *_property)
	{
		IntProperty *p=(IntProperty*)_property;
		p->set(get());
	}
	virtual Property *clone()
	{
		IntProperty *ret=new IntProperty(property_name,data_min,data_max);
		ret->set(get());
		return (Property*)ret;
	}
protected:
	int data;
	int data_min,data_max;
public:
	IntProperty(wchar *_property_name,int _data_min,int _data_max);
	virtual ~IntProperty();
	int get();
	void set(int _data);
	void setRange(int _data_min,int _data_max);
	bool isRange(int _data);
public:
	int operator = (int right){	set(right); return get(); }
	operator int() const{ return data; }
};


// ** ** ** ** CharsProperty ** ** ** ** //

#define CHARS_PROPERTY 1

class CharsProperty : public Property{
public:
	virtual Window *createWindow(int x,int y,int w,int h,Window *_parent);
	virtual int getControlH(){ return 22; }
	virtual void setToWindow(Window *wnd);
	virtual bool getFromWindow(Window *wnd);
	virtual bool isValidWindowValue(Window *wnd);
	virtual void messageRule(Window *owner);
public:
	virtual int write(FILE *fp);
	virtual int read(FILE *fp);
	virtual void copyDataTo(Property *_property)
	{
		CharsProperty *p=(CharsProperty*)_property;
		p->set(get());
	}
	virtual Property *clone()
	{
		CharsProperty *ret=new CharsProperty(property_name,size,size_min);
		ret->set(get());
		return (Property*)ret;
	}
protected:
	wchar *data;
	int size;
	int size_min;
public:
	CharsProperty(wchar *_property_name,int _size,int _size_min=0);
	virtual ~CharsProperty();
	wchar *get();
	void set(const wstring& _data);
public:
	wchar *operator = (wchar *right){	set(right); return get(); }
	operator wchar*() const{ return data; }
	operator wstring() const{ return wstring(data); }
public:
	bool equals(const wchar *str){ return wcscmp(data,str)==0; }
};

// ** ** ** ** EnumProperty ** ** ** ** //

#define ENUM_PROPERTY 2

class EnumProperty : public Property{
public:
	virtual Window *createWindow(int x,int y,int w,int h,Window *_parent);
	virtual int getControlH(){ return 22; }
	virtual void setToWindow(Window *wnd);
	virtual bool getFromWindow(Window *wnd);
	virtual bool isValidWindowValue(Window *wnd);
	virtual void messageRule(Window *owner);
public:
	virtual int write(FILE *fp);
	virtual int read(FILE *fp);
	virtual void copyDataTo(Property *_property)
	{
		EnumProperty *p=(EnumProperty*)_property;
		p->set(get());
	}
	virtual Property *clone()
	{
		EnumProperty *ret=new EnumProperty(property_name,elementlist);
		ret->set(get());
		return (Property*)ret;
	}
protected:
	StringList *elementlist;
	int data;
public:
	EnumProperty(wchar *_property_name,StringList *_elementlist);
	EnumProperty(wchar *_property_name,wchar *element0,...);
	virtual ~EnumProperty();
	int get();
	void set(int _selected);
public:
	int operator = (int right){	set(right); return get(); }
	operator int() const{ data; }
};
//*/


#endif
