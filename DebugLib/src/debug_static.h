#pragma once

//静的なアサート
template <bool> class StaticAssertClass{
public:
	StaticAssertClass(...){}
};
template<> class StaticAssertClass<false>{
public:
	StaticAssertClass(){}
};
class STATIC_ASSERT_ERR{};

//グローバル領域でしか使えない
#define STATIC_ASSERT(exp) \
namespace{ \
	void dummy(){ \
		StaticAssertClass<(exp)!=0> a( (STATIC_ASSERT_ERR()) ); \
	}; \
}

//ローカル領域でも使えるけど警告が出る
#define STATIC_ASSERT2(exp) \
class{ \
	void dummy(){ \
		StaticAssertClass<(exp)!=0> a( (STATIC_ASSERT_ERR()) ); \
	}; \
};

#define STATIC_ASSERT_MSG(exp,msg) \
namespace{ \
	class STATIC_ASSERT_ERR_#msg{}; \
	void dummy(){ \
		StaticAssertClass<(exp)!=0> a( (STATIC_ASSERT_ERR_#msg()) ); \
	}; \
}
