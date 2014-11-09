#pragma once

#include <windows.h> //HDC
#include <MathLib.h> //CRect, BoxSize
#include "CFont.h"
class CDib;

class CDcGraphics{
public:
	// -- -- HDC�̏������ƏI�������N���X -- -- //
	//���
	class DcImp{
	public:
		DcImp() : m_hdc(NULL) { }
		virtual ~DcImp(){}
		HDC _GetHDC() const{ return m_hdc; }
		math::CRect _GetRect() const{ return m_rc; }
	protected:
		void _Setup(HDC hdc){ m_hdc = hdc; }
		void _Setup(math::CRect rc){ m_rc = rc; }
	private:
		HDC		m_hdc;
		math::CRect	m_rc;
	};
	//���b�v�̂�
	class DcImp_Wrap : public DcImp{
	public:
		DcImp_Wrap(HDC hdc, const math::CRect& rc);
		~DcImp_Wrap();
	};
	//WM_PAINT���b�Z�[�W����
	class DcImp_Message : public DcImp{
	public:
		DcImp_Message(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		~DcImp_Message();
	private:
		HWND		m_hwnd;
		PAINTSTRUCT	m_ps;
	};
	//������DC
	class DcImp_Memory : public DcImp{
	public:
		DcImp_Memory(const math::CBoxSize& cBoxSize);
		~DcImp_Memory();
	private:
		HBITMAP	m_hbmp;
		HBITMAP	m_hbmpOrg;
	};
public:
	//�������ƏI��
	CDcGraphics(HDC hdc, const math::CRect& rc)
	{
		_BeforeInit();
		m_pcImp = new DcImp_Wrap(hdc, rc);
		_AfterInit();
	}
	CDcGraphics(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		_BeforeInit();
		m_pcImp = new DcImp_Message(hwnd, msg, wParam, lParam);
		_AfterInit();
	}
	CDcGraphics(const math::CBoxSize& cBoxSize)
	{
		_BeforeInit();
		m_pcImp = new DcImp_Memory(cBoxSize);
		_AfterInit();
	}
	~CDcGraphics()
	{
		//HDC�폜
		delete m_pcImp;
		//�`��I�u�W�F�N�g�폜
		delete m_pcFont;
	}
	//�������ƏI���F����
	void _BeforeInit()
	{
		m_pcImp = NULL;
		m_hdc = NULL;
		m_pcFont = NULL;
	}
	void _AfterInit();
public:
	// �}�`�`��
	void FillRect(const math::CRect& rc, COLORREF color);
	void DrawRect(const math::CRect& rc, COLORREF color);
	void DrawLine(const math::CPoint& ptStart, const math::CPoint& ptEnd, COLORREF color);
	// �e�L�X�g�`��
	void SetTextFont(const wchar_t* szFace, const CPixel& cHeight);
	void SetTextColor(COLORREF color);
	void DrawText(const wchar_t* szText, const math::CPoint& ptPos);
	// �C���[�W�`��
	void DrawImage(const math::CRect& rcDst, const CDib& cDib, const math::CRect& rcSrc);
	void DrawImage(const math::CRect& rcDst, HDC hdc, const math::CRect& rcSrc);
	// �����擾
	math::CRect	GetRect() const	{ return m_rc; }
	HDC		GetHDC() const	{ return m_hdc; }
private:
	//HDC����
	DcImp*		m_pcImp;
	HDC			m_hdc;		//m_pcImp�����_GetHDC()�Q�Ƃ�ێ�����B
	math::CRect		m_rc;
	//�`��I�u�W�F�N�g
	CFont*		m_pcFont;	//�K�v�ȂƂ��������������
};

