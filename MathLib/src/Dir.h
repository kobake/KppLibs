#pragma once

#include "_fwd.h"

//!�����N���X�B���W�n�� X���� Y����
class Dir{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	Dir() : d(0) { } //�f�t�H���g�ō�
	Dir(const Dir& rhs) : d(rhs.d) { }
	explicit Dir(int _d) : d(_d) { }
	explicit Dir(const math::CVec2& v);
	//���Z�q
	bool operator == (const Dir& rhs) const{ return d==rhs.d; }
	bool operator != (const Dir& rhs) const{ return d!=rhs.d; }
	Dir& operator = (const Dir& rhs){ d=rhs.d; return *this; }
	//�����ϊ�
	Dir leftHand() const{ return Dir((d-1+4)%4); }
	Dir rightHand() const{ return Dir((d+1+4)%4); }
	Dir backWard() const{ return Dir((d+2+4)%4); }
	//�����ϐ�
	int value() const{ return d; }
	//�ϊ�
	math::CVec2 toVec() const;
private:
	int d;
public:
	//�萔
};

//�C�����C���W�J����邱�Ƃ��F���āc
#define DIR_NONE Dir(-1)
#define DIR_L Dir(0)
#define DIR_U Dir(1)
#define DIR_R Dir(2)
#define DIR_D Dir(3)

/*
extern const Dir DIR_NONE;
extern const Dir DIR_L;
extern const Dir DIR_U;
extern const Dir DIR_R;
extern const Dir DIR_D;
*/
