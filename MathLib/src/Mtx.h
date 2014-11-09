#pragma once

#include <DebugLib_Assert.h>

namespace math{

template <int ROWS,int COLS> class Mtx{
private:
	typedef Mtx<ROWS,COLS> Me;
public:
	//�R���X�g���N�^
	Mtx()
	{
		clear();
	}
	void clear()
	{
		for(int i=0;i<ROWS;i++)for(int j=0;j<COLS;j++){
			table[i][j]=0;
		}
	}
	//�A�N�Z�X
	double& at(int i,int j)
	{
		assert(i>=0 && i<ROWS);
		assert(j>=0 && j<COLS);
		return table[i][j];
	}
	const double& at(int i,int j) const
	{
		assert(i>=0 && i<ROWS);
		assert(j>=0 && j<COLS);
		return table[i][j];
	}
	double* operator[](int i)
	{
		assert(i>=0 && i<=ROWS);
		return table[i];
	}
	const double* operator[](int i) const
	{
		assert(i>=0 && i<=ROWS);
		return table[i];
	}
	//�X�J���Ƃ̉��Z
	Me& operator *= (double p)
	{
		for(int i=0;i<ROWS;i++)for(int j=0;j<COLS;j++){
			at(i,j)*=p;
		}
		return *this;
	}
	Me& operator /= (double p)
	{
		for(int i=0;i<ROWS;i++)for(int j=0;j<COLS;j++){
			at(i,j)/=p;
		}
		return *this;
	}
	//�����s��Ƃ̉��Z
	Me& operator += (const Me& m)
	{
		for(int i=0;i<ROWS;i++)for(int j=0;j<COLS;j++){
			at(i,j)+=m.at(i,j);
		}
		return *this;
	}
	Me& operator -= (const Me& m)
	{
		for(int i=0;i<ROWS;i++)for(int j=0;j<COLS;j++){
			at(i,j)-=m.at(i,j);
		}
		return *this;
	}
	//���b�v
	Me operator + (const Me& m) const	{ Me ret=*this; return ret+=m; }
	Me operator - (const Me& m) const	{ Me ret=*this; return ret-=m; }
	Me operator * (double p) const		{ Me ret=*this; return ret*=p; }
	Me operator / (double p) const		{ Me ret=*this; return ret/=p; }
	Me operator - () const				{ Me ret=*this; return ret*=-1; }
	Me operator + () const				{ return *this; }
private:
	double table[ROWS][COLS];
};

//�s��ƍs��̏�Z
template <int ROWS1,int COLS1,int ROWS2,int COLS2>
Mtx<ROWS1,COLS2> mtxMultiply(const Mtx<ROWS1,COLS1>& m1,const Mtx<ROWS2,COLS2>& m2)
{
	STATIC_ASSERT2(COLS1==ROWS2);
	Mtx<ROWS1,COLS2> result;
	for(int i=0;i<ROWS1;i++){
		for(int j=0;j<COLS2;j++){
			result.at(i,j)=0;
			for(int k=0;k<COLS1;k++){
				result.at(i,j)+=m1.at(i,k)*m2.at(k,j);
			}
		}
	}
	return result;
}

//�悭�g���s��� typedef
typedef Mtx<4,4> Mtx44;
typedef Mtx<3,4> Mtx34;
typedef Mtx<3,3> Mtx33;

void mtx34_to_mtx44(Mtx44* mtx44,const Mtx34& mtx34);



} //namespace math

