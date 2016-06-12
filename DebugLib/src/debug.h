#pragma once

#include <BaseLib.h> // LTEXT
#include "DebugSystem.h"

#ifdef assert
	#undef assert
#endif

typedef void (*ASSERT_HANDLE)();

enum EDebugOutputMode{
	DEBUG_OUT_STDOUT,
	DEBUG_OUT_DEBUGGER,
	DEBUG_OUT_MSGBOX,
};

#ifdef _DEBUG
	const wchar_t* debug_format(const wchar_t* str,...);

	void debug_init(EDebugOutputMode mode, ASSERT_HANDLE pfAssertHandle = NULL);
	void debug_output(const char* str, ...);
	void debug_output(const wchar_t* str, ...);
	void debug_exit();

	void debug_on_assert();

	//�f�o�b�O������
	#define debugInit debug_init

	//�f�o�b�O�o��
	#define debugPrint debug_output

	//�ϐ��̓��e��\��
	#define debug_dump(V) debug_output("%s=%d\n",#V,V)

	//�G���[�ʒu�Ɠ��e���������b�Z�[�W��\�����ċ����I��
	#define assert(exp) \
	do{ \
		if(!(exp)){ \
			debug_output(L"!assert: %s(line%d): %s\n", LTEXT(__FILE__), __LINE__, LTEXT(#exp)); \
			debug_on_assert(); \
			debug_exit(); \
		} \
	}while(false)

	//�G���[�ʒu�ƔC�Ӄ��b�Z�[�W���o���ċ����I��
	#define assert_msg(exp, msg) \
	do{ \
		if(!(exp)){ \
			const wchar_t* p_msg=msg; \
			debug_output(L"!assert_msg: %s(line%d): %s -> %s\n", LTEXT(__FILE__), __LINE__, LTEXT(#exp), p_msg); \
			debug_on_assert(); \
			after_assert(); \
		} \
	}while(false)

	//�G���[�ʒu�Ɠ��e���������b�Z�[�W�������Ƃ��ė�O�𓊂���
	#define assert_exception(exp,CLASS) \
	do{ \
		if(!(exp)){ \
			const wchar_t* msg=debug_format("!assert_exception: %s(line%d): %s\n", __FILE__, __LINE__, #exp); \
			debug_on_assert(); \
			throw CLASS(msg); \
		} \
	}while(false)

	//�x���͏o�����ǋ����I���͂��Ȃ�
	#define assert_warning(exp) \
	do{ \
		if(!(exp)){ \
			debug_output(L"!assert_warning: %hs(line%d): %hs\n", __FILE__, __LINE__, #exp); \
			debug_on_assert(); \
		} \
	}while(false)

	//�G���[���O
	enum ELogLevel{
		LEVEL_INFO				= 0x01,	//���
		LEVEL_WARNING			= 0x02,	//�x��
		LEVEL_ERROR				= 0x04,	//�G���[
		LEVEL_FATAL				= 0x08,	//�[���ȃG���[
		LEVEL_DEVELOPING_ERROR	= 0x10,	//�J���҂��Ώ����ׂ��G���[
		LEVEL_ALL				= 0xFF,	//�ȏ�̂��ׂĂ�����
	};
	inline ELogLevel operator ~ (ELogLevel e)
	{
		return (ELogLevel)~((int)e);
	}
	inline ELogLevel operator | (ELogLevel e1, ELogLevel e2)
	{
		return (ELogLevel)((int)e1 | (int)e2);
	}
	inline ELogLevel operator & (ELogLevel e1, ELogLevel e2)
	{
		return (ELogLevel)((int)e1 & (int)e2);
	}
	void _ErrorLog(ELogLevel nLevel, const wchar_t* szMsg, ...);
	void _SetLogLevel(ELogLevel nLevel);
	#define ErrorLog _ErrorLog
	#define SetLogLevel _SetLogLevel



#else

	inline void debug_output_dummy(const wchar_t* str, ...){}
	inline void debug_output_dummy(const char* str, ...){}

	inline void debug_init_dummy(EDebugOutputMode mode, ASSERT_HANDLE pfAssertHandle = NULL){}
	#define debugInit debug_init_dummy
	#define debugPrint debug_output_dummy
	#define assert(exp)
	#define assertmsg(exp, msg)
	#define assert_exception(exp,CLASS)
	#define assert_warning

	#if _MSC_VER >= 1400
		#define ErrorLog(...)
		#define SetLogLevel(...)
	#else
		enum ELogLevel;
		inline void _ErrorLog(ELogLevel nLevel, const wchar_t* szMsg, ...){}
		inline void _SetLogLevel(ELogLevel nLevel){}
		#define ErrorLog _ErrorLog
		#define SetLogLevel _SetLogLevel
	#endif

#endif



