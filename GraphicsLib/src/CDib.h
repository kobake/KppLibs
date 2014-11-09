#pragma once

#include <windows.h>
#include <GraphicsLib.h> // Type
#include "std/myexception.h"
#include "CImage.h"
#include "sdk/libfwd_win.h"
#include <IoLib.h> //InputData
#include "std/mem.h"

class CColor;

class CDib : public CImage, public InputData{
public:
	//��O
	class DibException : public myexception{
	public:
		DibException(const wchar_t* msg) : myexception(msg) { }
	};
	class NotDib : public DibException{
	public:
		NotDib(const wchar_t* msg) : DibException(msg) { }
	};
private:
	//�R�s�[�֎~
	CDib(const CDib& rhs);
	CDib& operator = (const CDib& rhs);
private:
	//�����⏕
	void _construct(const math::CBoxSize& _size,int depth,int size_image);
	void _destroy();
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CDib();
	CDib(const BITMAPINFO& bmpinfo, const void* bits);
	CDib(const math::CBoxSize& _size,int depth);
	CDib(const wchar_t* path);
	virtual ~CDib();

	//�R�s�[
	CDib* Clone() const;

	//���
	void Assign(const BITMAPINFO& bmpinfo, const void* bits);
	void Assign(HDC hdc);

	//�ϊ�
	void Resize(const math::CBoxSize& new_size);

	//�`��
	void DrawTo(HDC hdc, const math::CPoint& ptDst);

	//�F����
	int GetBitsPerPixel() const{ return m_bmpinfo->bmiHeader.biBitCount; }
	int GetPaletteCount() const;

	//�X�g���[��
	void Read(CInputStream& in);

	//�]��
	void SetBits(const math::CPoint& ptDst,const CDib& dibSrc,const math::CRect& rcSrc);
	void SetPixel(const math::CPoint& pt,const CColor& color);
	CColor GetPixel(const math::CPoint& pt) const;

	//CImage�C���^�[�t�F�[�X
	math::CBoxSize GetSize() const;
	DibCarrier GetBitmapCarrier() const{ return DibCarrier(m_bmpinfo,m_bits); }

	//�����C���^�[�t�F�[�X
	const tagBITMAPINFO*	GetBitmapInfo() const{ return m_bmpinfo; }
	size_t					GetBitmapInfoSize() const;
	const void*				GetBits() const{ return m_bits; }
	size_t					GetBitsSize() const;
private:
	int			m_wbytes;
	BITMAPINFO*	m_bmpinfo;
	byte*		m_bits;
};

