#include "_required.h"
#include "Transformer3.h"
#include <cmath>

namespace math{

	
Mtx<3,4> mtxMultiply(const Mtx<3,4>& m1,const Mtx<3,4>& m2)
{
	Mtx<3,4> result;
	for(int i=0;i<3;i++){
		for(int j=0;j<4;j++){
			result.at(i,j)=0;
			for(int k=0;k<3;k++){
				result.at(i,j)+=m1.at(i,k)*m2.at(k,j);
			}
			if(j==3){
				result.at(i,j)+=m1.at(i,3);
			}
		}
	}
	return result;
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                       変換行列貯蓄                          //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

Transformer3::Transformer3()
{
	//単位行列
	mtxIdentity(&mtx);
}

void Transformer3::add(const Mtx<3,4>& m)
{
	mtx=mtxMultiply(m,mtx);
}
const Mtx<3,4>& Transformer3::getMtx(Mtx<3,4>* _mtx)
{
	if(_mtx==NULL){
		return mtx;
	}
	*_mtx=mtx;
	return *_mtx;
}




// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                       基本変換行列                          //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //


const Mtx<3,4>& mtxMake(
	Mtx<3,4>* mtx,
	double n00,double n01,double n02,double n03,
	double n10,double n11,double n12,double n13,
	double n20,double n21,double n22,double n23
){
	if(mtx==0){
		static Mtx<3,4> buf;
		mtx=&buf;
	}
	mtx->at(0,0)=n00;	mtx->at(0,1)=n01;	mtx->at(0,2)=n02;	mtx->at(0,3)=n03;
	mtx->at(1,0)=n10;	mtx->at(1,1)=n11;	mtx->at(1,2)=n12;	mtx->at(1,3)=n13;
	mtx->at(2,0)=n20;	mtx->at(2,1)=n21;	mtx->at(2,2)=n22;	mtx->at(2,3)=n23;
	return *mtx;
}

const Mtx<3,4>& mtxIdentity(Mtx<3,4>* mtx)
{
	mtx->clear();
	for(int i=0;i<3;i++){
		mtx->at(i,i)=1;
	}
	return *mtx;
}

const Mtx<3,4>& mtxTranslate(Mtx<3,4>* mtx,const Vec3Base<double>& p)
{
	if(mtx==0){
		static Mtx<3,4> buf;
		mtx=&buf;
	}
	mtxIdentity(mtx);
	mtx->at(0,3)=p.x;
	mtx->at(1,3)=p.y;
	mtx->at(2,3)=p.z;
	return *mtx;
}

const Mtx<3,4>& mtxRotate(Mtx<3,4>* mtx,const Vec3Base<double>& p)
{
	if(mtx==0){
		static Mtx<3,4> buf;
		mtx=&buf;
	}
	Transformer3 t;
	t.add(mtxPitch(p.x));
	t.add(mtxYaw(p.y));
	t.add(mtxRoll(p.z));
	t.getMtx(mtx);
	return *mtx;
}

const Mtx<3,4>& mtxScale(Mtx<3,4>* mtx,const Vec3Base<double>& p)
{
	if(mtx==0){
		static Mtx<3,4> buf;
		mtx=&buf;
	}
	mtxMake(mtx,
		p.x,			0,				0,				0,
		0,				p.y,			0,				0,
		0,				0,				p.z,			0
		);
	return *mtx;
}


Mtx<3,4> mtxScale(double power)
{
	return mtxMake(NULL,
		power,			0,				0,				0,
		0,				power,			0,				0,
		0,				0,				power,			0
		);
}

Mtx<3,4> mtxRoll(double angle)
{
	return mtxMake(NULL,
		cos(angle),		-sin(angle),	0,				0,
		sin(angle),		cos(angle),		0,				0,
		0,				0,				1,				0
		);
}

Mtx<3,4> mtxYaw(double angle)
{
	return mtxMake(NULL,
		cos(angle),		0,				sin(angle),		0,
		0,				1,				0,				0,
		-sin(angle),	0,				cos(angle),		0
		);
}

Mtx<3,4> mtxPitch(double angle)
{
	return mtxMake(NULL,
		1,				0,				0,				0,
		0,				cos(angle),		-sin(angle),	0,
		0,				sin(angle),		cos(angle),		0
		);
}




} //namespace math
