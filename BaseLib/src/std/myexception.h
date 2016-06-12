#pragma once

#include <string>
#include <exception>
//#include <BaseLib.h> //LTEXT

//!���C�h�������what�����������O�̋K��N���X
/*!
�R���X�g���N�^�ɕ�����|�C���^��n�����Ƃ��ł���B
�����܂ł��|�C���^��ێ����邾���ł���A�����񂻂̂��͕̂ێ����Ȃ��̂Œ��ӁB
*/
class wexception : public std::exception{
public:
	wexception(const wchar_t* _str=L"unknown") : m_str(_str) { } //!<���e������n�����Ƃ��ł���B�C���X�^���X���ɂ̓��b�Z�[�W��ێ����Ȃ��̂Œ��ӁB
	virtual ~wexception() throw(){}
	virtual const wchar_t* what_w() const throw(){ return m_str; }
protected:
	virtual const char* what() const throw();
private:
	const wchar_t* m_str;
};

class myexception : public std::exception{
public:
	myexception(const char* format, ...);
	myexception(const wchar_t* format, ...);
	const char* what() const{ return m_buf; }
	void assignv(const wchar_t* _str, va_list& mark);
private:
	char m_buf[256];
};


class mywexception : public wexception{
private:
	typedef std::wstring wstring;
public:
	mywexception(const wchar_t* _str, ...);
	void assignv(const wchar_t* _str, va_list& mark);
	const wchar_t* what_w() const throw(){ return str; }
private:
	wchar_t str[256];
};

#include <stdarg.h>

#define GEN_MYEXCEPTION(CLASS) \
class CLASS : public myexception{ \
public: \
	CLASS() \
	: myexception(LTEXT(#CLASS)) \
	{ \
	} \
	CLASS(const wchar_t* msg, ...) \
	: myexception(L"") \
	{ \
		va_list mark; \
		va_start(mark,msg); \
		assignv(msg,mark); \
		va_end(mark); \
	} \
}


#define GEN_MYEXCEPTION2(CLASS, PARENT) \
class CLASS : public PARENT{ \
public: \
	CLASS() \
	: PARENT(LTEXT(#CLASS)) \
	{ \
	} \
	CLASS(const wchar_t* msg, ...) \
	: PARENT(L"") \
	{ \
		va_list mark; \
		va_start(mark,msg); \
		assignv(msg,mark); \
		va_end(mark); \
	} \
}


#define GEN_ARGSEXCEPTION(CLASS,PARENT) \
class CLASS : public PARENT{ \
public: \
	CLASS() \
	: PARENT(LTEXT(#CLASS)) \
	{ \
	} \
	CLASS(const wchar_t* msg, ...) \
	: PARENT(L"") \
	{ \
		va_list mark; \
		va_start(mark,msg); \
		assignv(msg,mark); \
		va_end(mark); \
	} \
}





#define GEN_EXCEPTION(CLASS,WHAT) \
class CLASS : public wexception{ \
public: \
	CLASS(const wchar_t* szMsg = WHAT) : wexception(szMsg) { } \
};

