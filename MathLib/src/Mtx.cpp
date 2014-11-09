#include "_required.h"
#include "Mtx.h"

namespace math{

//変換用行列の生成 (4行目を 0,0,0,1 とする)
void mtx34_to_mtx44(Mtx44* mtx44,const Mtx34& mtx34)
{
	for(int i=0;i<3;i++){ //行
		for(int j=0;j<4;j++){ //列
			mtx44->at(i,j)=mtx34.at(i,j);
		}
	}
	for(int j=0;j<3;j++){
		mtx44->at(3,j)=0;
	}
	mtx44->at(3,3)=1;
}


} //namespace math
