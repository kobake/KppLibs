#pragma once

#include <BaseLib.h>
#include "../../MathLib/MathLib.h"
#include <windows.h>


//!�C���[�W�N���X�B
/*!
������p������N���X�́A�C���[�W�]���C���^�[�t�F�[�X���Œ�1�̓I�[�o�[���C�h����K�v������B
*/
class CImage{
public:
	//�e��]���v���g�R��
	struct DibCarrier{
		const BITMAPINFO* bmpinfo;
		const void* bits;

		DibCarrier() : bmpinfo(NULL), bits(NULL) { }
		DibCarrier(const BITMAPINFO* _bmpinfo,const void* _bits) : bmpinfo(_bmpinfo), bits(_bits) { }
		bool isValid() const{ return bmpinfo!=NULL && bits!=NULL; }
	};
	struct DcCarrier{
		HDC hdc;

		DcCarrier(){ hdc=NULL; }
		DcCarrier(HDC _hdc) : hdc(_hdc) { }
		bool isValid() const{ return hdc!=NULL; }
	};
	struct BmpCarrier{
		HBITMAP hbmp;

		BmpCarrier(){ hbmp=NULL; }
		BmpCarrier(HBITMAP _hbmp) : hbmp(_hbmp) { }
		bool isValid() const{ return hbmp!=NULL; }
	};
private:
	//����֎~
	CImage(const CImage&);
	CImage& operator = (const CImage&);
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CImage(){}
	virtual ~CImage(){}
	//��{���擾
	virtual math::CBoxSize GetSize() const=0;
	//�C���[�W�]���C���^�[�t�F�[�X
	virtual DibCarrier GetBitmapCarrier()	const{ return DibCarrier(); }
	virtual DcCarrier GetDcCarrier()		const{ return DcCarrier(); }
	virtual BmpCarrier GetBmpCarrier()		const{ return BmpCarrier(); }
};

class DcImage{
};
