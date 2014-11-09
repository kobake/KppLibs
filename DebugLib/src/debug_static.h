#pragma once

//�ÓI�ȃA�T�[�g
template <bool> class StaticAssertClass{
public:
	StaticAssertClass(...){}
};
template<> class StaticAssertClass<false>{
public:
	StaticAssertClass(){}
};
class STATIC_ASSERT_ERR{};

//�O���[�o���̈�ł����g���Ȃ�
#define STATIC_ASSERT(exp) \
namespace{ \
	void dummy(){ \
		StaticAssertClass<(exp)!=0> a( (STATIC_ASSERT_ERR()) ); \
	}; \
}

//���[�J���̈�ł��g���邯�ǌx�����o��
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
