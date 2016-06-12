#include "_required.h"
#include "Intersect3.h"
#include "Mtx.h"
#include "Obb3.h"
#include "Vec3.h"
#include "Aabb3.h"
#include "Segment3.h"

#define EPSILON 0.001 //##�K��
#define tabs util::tabs

namespace math{

bool Intersect3::check_intersect(const Obb3& obb0,const Obb3& obb1)
{
	const Obb3& a=obb0;
	const Obb3& b=obb1;
	typedef Obb3::t_Vec3 t_Vec3;
	//
	double ra,rb;
	Mtx33 R,AbsR;
	// a �̍��W�t���[���̒��� b ��\�������]�s����v�Z
	for(int i=0;i<3;i++){
		for(int j=0;j<3;j++){
			R[i][j]=dot_product(a.u[i],b.u[j]);
		}
	}
	//���s�ړ��x�N�g�� t ���v�Z
	t_Vec3 t=b.c-a.c;
	//���s�ړ��� a �̍��W�t���[���ɕϊ�
	t = t_Vec3(dot_product(t,a.u[0]),dot_product(t,a.u[1]), dot_product(t,a.u[2]));

	//���ʂ̕��������v�Z
	//2�̕ӂ����s�ł����̊O�ς��[���x�N�g��(���邢�͂���ɋ߂��x�N�g��)�ɂȂ鎞��
	//���Z�G���[���N���Ȃ��悤�ɃC�v�V�����̍���ǉ�(�ڂ����͖{�����Q��)
	for(int i=0;i<3;i++){
		for(int j=0;j<3;j++){
			AbsR[i][j]=tabs(R[i][j])+EPSILON;
		}
	}

	//�� L=A0, L=A1, L=A2 �𔻒�
	for(int i=0;i<3;i++){
		ra=a.e[i];
		rb=
			b.e[0]*AbsR[i][0]+
			b.e[1]*AbsR[i][1]+
			b.e[2]*AbsR[i][2];
		if(tabs(t[i])>ra+rb)return false;
	}

	//�� L=B0, L=B1, L=B2 �𔻒�
	for(int i=0;i<3;i++){
		ra=
			a.e[0]*AbsR[0][i]+
			a.e[1]*AbsR[1][i]+
			a.e[2]*AbsR[2][i];
		rb=b.e[i];
		if(
			tabs(
				t[0]*R[0][i]+
				t[1]*R[1][i]+
				t[2]*R[2][i]
			)
			>
			ra+rb
		)
		{
			return false;
		}
	}

	// -- -- �����̍s�ɒB�������_�ŏՓ˂��������Ă���\���͂��Ȃ荂��(������true��Ԃ��Ă��܂��Ă��L�p�ȃP�[�X�͂���) -- -- //

    // ��L = A0 x B0�𔻒�
    ra = a.e[1] * AbsR[2][0] + a.e[2] * AbsR[1][0];
    rb = b.e[1] * AbsR[0][2] + b.e[2] * AbsR[0][1];
    if (tabs(t[2] * R[1][0] - t[1] * R[2][0]) > ra + rb) return false;

    // ��L = A0 x B1�𔻒�
    ra = a.e[1] * AbsR[2][1] + a.e[2] * AbsR[1][1];
    rb = b.e[0] * AbsR[0][2] + b.e[2] * AbsR[0][0];
    if (tabs(t[2] * R[1][1] - t[1] * R[2][1]) > ra + rb) return false;

    // ��L = A0 x B2�𔻒�
    ra = a.e[1] * AbsR[2][2] + a.e[2] * AbsR[1][2];
    rb = b.e[0] * AbsR[0][1] + b.e[1] * AbsR[0][0];
    if (tabs(t[2] * R[1][2] - t[1] * R[2][2]) > ra + rb) return false;

    // ��L = A1 x B0�𔻒�
    ra = a.e[0] * AbsR[2][0] + a.e[2] * AbsR[0][0];
    rb = b.e[1] * AbsR[1][2] + b.e[2] * AbsR[1][1];
    if (tabs(t[0] * R[2][0] - t[2] * R[0][0]) > ra + rb) return false;

    // ��L = A1 x B1�𔻒�
    ra = a.e[0] * AbsR[2][1] + a.e[2] * AbsR[0][1];
    rb = b.e[0] * AbsR[1][2] + b.e[2] * AbsR[1][0];
    if (tabs(t[0] * R[2][1] - t[2] * R[0][1]) > ra + rb) return false;

    // ��L = A1 x B2�𔻒�
    ra = a.e[0] * AbsR[2][2] + a.e[2] * AbsR[0][2];
    rb = b.e[0] * AbsR[1][1] + b.e[1] * AbsR[1][0];
    if (tabs(t[0] * R[2][2] - t[2] * R[0][2]) > ra + rb) return false;

    // ��L = A2 x B0�𔻒�
    ra = a.e[0] * AbsR[1][0] + a.e[1] * AbsR[0][0];
    rb = b.e[1] * AbsR[2][2] + b.e[2] * AbsR[2][1];
    if (tabs(t[1] * R[0][0] - t[0] * R[1][0]) > ra + rb) return false;

    // ��L = A2 x B1�𔻒�
    ra = a.e[0] * AbsR[1][1] + a.e[1] * AbsR[0][1];
    rb = b.e[0] * AbsR[2][2] + b.e[2] * AbsR[2][0];
    if (tabs(t[1] * R[0][1] - t[0] * R[1][1]) > ra + rb) return false;

    // ��L = A2 x B2�𔻒�
    ra = a.e[0] * AbsR[1][2] + a.e[1] * AbsR[0][2];
    rb = b.e[0] * AbsR[2][1] + b.e[1] * AbsR[2][0];
    if (tabs(t[1] * R[0][2] - t[0] * R[1][2]) > ra + rb) return false;

    // ��������������Ȃ��̂ŁAOBB�͌������Ă���
    return true;
}

bool Intersect3::check_intersect(const Aabb3& aabb,const Segment3& segline)
{
	typedef Aabb3::t_Vec3 t_Vec3;
	//����
	const t_Vec3& p0=segline.getBegin();
	const t_Vec3& p1=segline.getEnd();
	const Aabb3& b=aabb;

	//��������v�Z

	t_Vec3 c=b.getCenter(); //�{�b�N�X�̒��S�_
	t_Vec3 e=b.getMax()-c;  //�{�b�N�X�̕��̔����͈̔�
	t_Vec3 m=(p0+p1)*0.5;   //�����̒��_
	t_Vec3 d=p1-m;          //�����̒����̔����̃x�N�g��
	m=m-c;                  //�{�b�N�X�Ɛ��������_�܂ŕ��s�ړ�
	//���[���h���W�����������ł��邩�ǂ�������
	float adx=(float)tabs(d.x);
	if(tabs(m.x)>e.x+adx)return false;
	float ady=(float)tabs(d.y);
	if(tabs(m.y)>e.y+ady)return false;
	float adz=(float)tabs(d.z);
	if(tabs(m.z)>e.z+adz)return false;
	//���������W���ɕ��s(���邢�͂���ɋ߂����)�̎���
	//�C�v�V�����̍���ǉ����ĉ��Z�̌��̉e������߂�(�ڍׂ̓e�L�X�g�Q��)
	adx+=(float)EPSILON; ady+=(float)EPSILON; adz+=(float)EPSILON;
	//�����̕����x�N�g���̊O�ς����W���ɑ΂��Ď���
	if(tabs(m.y*d.z-m.z*d.y)>e.y*adz+e.z*ady)return false;
	if(tabs(m.z*d.x-m.x*d.z)>e.x*adz+e.z*adx)return false;
	if(tabs(m.x*d.y-m.y*d.x)>e.x*ady+e.y*adx)return false;
	//��������������Ȃ������̂ŁA������AABB�ɏd�Ȃ��Ă���
	return true;
}


} //namespace math
