#include "_required.h"
#include "Mtx.h"

namespace math{

//�ϊ��p�s��̐��� (4�s�ڂ� 0,0,0,1 �Ƃ���)
void mtx34_to_mtx44(Mtx44* mtx44,const Mtx34& mtx34)
{
	for(int i=0;i<3;i++){ //�s
		for(int j=0;j<4;j++){ //��
			mtx44->at(i,j)=mtx34.at(i,j);
		}
	}
	for(int j=0;j<3;j++){
		mtx44->at(3,j)=0;
	}
	mtx44->at(3,3)=1;
}


} //namespace math
