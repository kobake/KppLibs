#pragma once

#include "sys/math2/MathVec2.h"

template <int N> class CompassBase{
public:
	typedef CompassBase<N> Me;
protected:
	//�Ȃ��� explicit ���g���Ȃ��c
	CompassBase<N>(int _d) : d(_d) { }
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CompassBase<N>() : d(0) { } //�f�t�H���g�ō�
	CompassBase<N>(const Me& rhs) : d(rhs.d) { }
	//���Z�q
	bool operator == (const Me& rhs) const{ return d==rhs.d; }
	bool operator != (const Me& rhs) const{ return d!=rhs.d; }
	Me& operator = (const Me& rhs){ d=rhs.d; return *this; }
	//�����ϊ�
	Me leftHand() const{ return Me((d-1+4)%4); }
	Me rightHand() const{ return Me((d+1+4)%4); }
	Me backWard() const{ return Me((d+2+4)%4); }
	//�����ϐ�
	int value() const{ return d; }
	//�c�[��
	MathVec2 getVec() const
	{
		switch(d){
		case 0: return MathVec2(-1, 0);
		case 1: return MathVec2(0, -1);
		case 2: return MathVec2(1, 0);
		case 3: return MathVec2(0, 1);
		}
		return MathVec2(0,0);
	}
	f32 getYaw() const
	{
		return (f32)((2-d)*PI/2);
	}
	//�t�@�N�g���[
	static Me create(int _d){ return CompassBase<N>((_d+4+4)%4); }
private:
	int d;
};

//��Ε��p
class Compass : public CompassBase<0>{
protected:
	//�Ȃ��� explicit ���g���Ȃ��c
	Compass(int _d) : CompassBase<0>(_d) { }
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	Compass() : CompassBase<0>(0) { } //�f�t�H���g�ō�
	Compass(const Me& rhs) : CompassBase<0>(rhs) { }
	//�f�o�b�O
	const char* getName() const;
public:
	//�萔
	static const Compass West		;
	static const Compass North		;
	static const Compass East		;
	static const Compass South		;
	static const Compass NoCompass	;
};

//���Ε��p
class Course : public CompassBase<1>{
protected:
	//�Ȃ��� explicit ���g���Ȃ��c
	Course(int _d) : CompassBase<1>(_d) { }
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	Course() : CompassBase<1>(0) { } //�f�t�H���g�ō�
	Course(const Me& rhs) : CompassBase<1>(rhs) { }
public:
	//�萔
	static const Course Backward	; //���
	static const Course Lefthand	; //�������
	static const Course Forward		; //�O��
	static const Course Righthand	; //�E�����
	static const Course NoCourse	;
};

