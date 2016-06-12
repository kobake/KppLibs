#include "_required.h"
#include "Intersect3.h"
#include "Mtx.h"
#include "Obb3.h"
#include "Vec3.h"
#include "Aabb3.h"
#include "Segment3.h"

#define EPSILON 0.001 //##適当
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
	// a の座標フレームの中で b を表現する回転行列を計算
	for(int i=0;i<3;i++){
		for(int j=0;j<3;j++){
			R[i][j]=dot_product(a.u[i],b.u[j]);
		}
	}
	//平行移動ベクトル t を計算
	t_Vec3 t=b.c-a.c;
	//平行移動を a の座標フレームに変換
	t = t_Vec3(dot_product(t,a.u[0]),dot_product(t,a.u[1]), dot_product(t,a.u[2]));

	//共通の部分式を計算
	//2つの辺が平行でそれらの外積がゼロベクトル(あるいはそれに近いベクトル)になる時に
	//演算エラーが起きないようにイプシロンの項を追加(詳しくは本文を参照)
	for(int i=0;i<3;i++){
		for(int j=0;j<3;j++){
			AbsR[i][j]=tabs(R[i][j])+EPSILON;
		}
	}

	//軸 L=A0, L=A1, L=A2 を判定
	for(int i=0;i<3;i++){
		ra=a.e[i];
		rb=
			b.e[0]*AbsR[i][0]+
			b.e[1]*AbsR[i][1]+
			b.e[2]*AbsR[i][2];
		if(tabs(t[i])>ra+rb)return false;
	}

	//軸 L=B0, L=B1, L=B2 を判定
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

	// -- -- ※この行に達した時点で衝突が発生している可能性はかなり高い(ここでtrueを返してしまっても有用なケースはある) -- -- //

    // 軸L = A0 x B0を判定
    ra = a.e[1] * AbsR[2][0] + a.e[2] * AbsR[1][0];
    rb = b.e[1] * AbsR[0][2] + b.e[2] * AbsR[0][1];
    if (tabs(t[2] * R[1][0] - t[1] * R[2][0]) > ra + rb) return false;

    // 軸L = A0 x B1を判定
    ra = a.e[1] * AbsR[2][1] + a.e[2] * AbsR[1][1];
    rb = b.e[0] * AbsR[0][2] + b.e[2] * AbsR[0][0];
    if (tabs(t[2] * R[1][1] - t[1] * R[2][1]) > ra + rb) return false;

    // 軸L = A0 x B2を判定
    ra = a.e[1] * AbsR[2][2] + a.e[2] * AbsR[1][2];
    rb = b.e[0] * AbsR[0][1] + b.e[1] * AbsR[0][0];
    if (tabs(t[2] * R[1][2] - t[1] * R[2][2]) > ra + rb) return false;

    // 軸L = A1 x B0を判定
    ra = a.e[0] * AbsR[2][0] + a.e[2] * AbsR[0][0];
    rb = b.e[1] * AbsR[1][2] + b.e[2] * AbsR[1][1];
    if (tabs(t[0] * R[2][0] - t[2] * R[0][0]) > ra + rb) return false;

    // 軸L = A1 x B1を判定
    ra = a.e[0] * AbsR[2][1] + a.e[2] * AbsR[0][1];
    rb = b.e[0] * AbsR[1][2] + b.e[2] * AbsR[1][0];
    if (tabs(t[0] * R[2][1] - t[2] * R[0][1]) > ra + rb) return false;

    // 軸L = A1 x B2を判定
    ra = a.e[0] * AbsR[2][2] + a.e[2] * AbsR[0][2];
    rb = b.e[0] * AbsR[1][1] + b.e[1] * AbsR[1][0];
    if (tabs(t[0] * R[2][2] - t[2] * R[0][2]) > ra + rb) return false;

    // 軸L = A2 x B0を判定
    ra = a.e[0] * AbsR[1][0] + a.e[1] * AbsR[0][0];
    rb = b.e[1] * AbsR[2][2] + b.e[2] * AbsR[2][1];
    if (tabs(t[1] * R[0][0] - t[0] * R[1][0]) > ra + rb) return false;

    // 軸L = A2 x B1を判定
    ra = a.e[0] * AbsR[1][1] + a.e[1] * AbsR[0][1];
    rb = b.e[0] * AbsR[2][2] + b.e[2] * AbsR[2][0];
    if (tabs(t[1] * R[0][1] - t[0] * R[1][1]) > ra + rb) return false;

    // 軸L = A2 x B2を判定
    ra = a.e[0] * AbsR[1][2] + a.e[1] * AbsR[0][2];
    rb = b.e[0] * AbsR[2][1] + b.e[1] * AbsR[2][0];
    if (tabs(t[1] * R[0][2] - t[0] * R[1][2]) > ra + rb) return false;

    // 分離軸が見つからないので、OBBは交差している
    return true;
}

bool Intersect3::check_intersect(const Aabb3& aabb,const Segment3& segline)
{
	typedef Aabb3::t_Vec3 t_Vec3;
	//引数
	const t_Vec3& p0=segline.getBegin();
	const t_Vec3& p1=segline.getEnd();
	const Aabb3& b=aabb;

	//ここから計算

	t_Vec3 c=b.getCenter(); //ボックスの中心点
	t_Vec3 e=b.getMax()-c;  //ボックスの幅の半分の範囲
	t_Vec3 m=(p0+p1)*0.5;   //線分の中点
	t_Vec3 d=p1-m;          //線分の長さの半分のベクトル
	m=m-c;                  //ボックスと線分を原点まで平行移動
	//ワールド座標軸が分離軸であるかどうか試す
	float adx=(float)tabs(d.x);
	if(tabs(m.x)>e.x+adx)return false;
	float ady=(float)tabs(d.y);
	if(tabs(m.y)>e.y+ady)return false;
	float adz=(float)tabs(d.z);
	if(tabs(m.z)>e.z+adz)return false;
	//線分が座標軸に平行(あるいはそれに近い状態)の時に
	//イプシロンの項を追加して演算の誤りの影響を弱める(詳細はテキスト参照)
	adx+=(float)EPSILON; ady+=(float)EPSILON; adz+=(float)EPSILON;
	//線分の方向ベクトルの外積を座標軸に対して試す
	if(tabs(m.y*d.z-m.z*d.y)>e.y*adz+e.z*ady)return false;
	if(tabs(m.z*d.x-m.x*d.z)>e.x*adz+e.z*adx)return false;
	if(tabs(m.x*d.y-m.y*d.x)>e.x*ady+e.y*adx)return false;
	//分離軸が見つからなかったので、線分はAABBに重なっている
	return true;
}


} //namespace math
