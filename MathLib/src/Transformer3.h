#pragma once

#include "Mtx.h"
#include "Vec3.h"

namespace math{

//•ÏŠ·s—ñ’™’~
class Transformer3{
public:
	Transformer3();
	void add(const Mtx<3,4>& m);
	const Mtx<3,4>& getMtx(Mtx<3,4>* mtx=NULL);
private:
	Mtx<3,4> mtx;
};

//s—ñ¶¬
const Mtx<3,4>& mtxMake(
	Mtx<3,4>* mtx,
	double n00,double n01,double n02,double n03,
	double n10,double n11,double n12,double n13,
	double n20,double n21,double n22,double n23
);
const Mtx<3,4>& mtxIdentity(Mtx<3,4>* mtx);

//•ÏŠ·s—ñ¶¬
const Mtx<3,4>& mtxTranslate(Mtx<3,4>* mtx,const Vec3Base<double>& p);
const Mtx<3,4>& mtxRotate(Mtx<3,4>* mtx,const Vec3Base<double>& p);
const Mtx<3,4>& mtxScale(Mtx<3,4>* mtx,const Vec3Base<double>& p);
Mtx<3,4> mtxScale(double power);

Mtx<3,4> mtxRoll(double angle);
Mtx<3,4> mtxYaw(double angle);
Mtx<3,4> mtxPitch(double angle);


} //namespace math
