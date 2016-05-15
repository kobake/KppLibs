#pragma once

#include "math/Vec3.h"

//�擾
class ConstObj3{
public:
	typedef Vec3Base<double> t_Vec3;
public:
	virtual ~ConstObj3(){}
	virtual void getPos(t_Vec3* _pos) const=0;
	//���b�v
	t_Vec3 getPos() const
	{
		t_Vec3 pos;
		getPos(&pos);
		return pos;
	}
};

//�擾�Ɛݒ�
class Obj3 : public ConstObj3{
public:
	using ConstObj3::getPos;
public:
	virtual ~Obj3(){}
	virtual void setPos(const t_Vec3& _pos)=0;
	//���b�v
	void offsetPos(const t_Vec3& off){ setPos(getPos()+off); }
};
