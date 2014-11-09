#pragma once

#include <windows.h> //HBITMAP
#include "CImage.h"
#include <MathLib.h> //BoxSize

class CBitmap : public CImage{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CBitmap(HINSTANCE hInst,int id);
	CBitmap(HBITMAP _hBitmap,bool _hold);
	CBitmap();
	virtual ~CBitmap();
	//�C���^�[�t�F�[�X
	HBITMAP getHBITMAP(){ return m_hbmp; }

	// -- -- CImage�C���^�[�t�F�[�X -- -- //
	//��{���擾
	math::CBoxSize GetSize() const{ return m_size; }
	//�C���[�W�]���C���^�[�t�F�[�X
	BmpCarrier GetBmpCarrier() const{ return BmpCarrier(m_hbmp); }

protected:
	//�����⏕
	void _calcSize(); //!< �T�C�Y���擾���ĕێ�
private:
	HBITMAP m_hbmp;
	math::CBoxSize m_size;
	bool m_hold;
};




