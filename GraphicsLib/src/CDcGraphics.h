#pragma once

#include <windows.h> //HDC
#include <MathLib.h> //CRect, BoxSize
#include "CFont.h"
class CDib;

class CDcGraphics{
public:
	// -- -- HDCの初期化と終了処理クラス -- -- //
	//基底
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
	//ラップのみ
	class DcImp_Wrap : public DcImp{
	public:
		DcImp_Wrap(HDC hdc, const math::CRect& rc);
		~DcImp_Wrap();
	};
	//WM_PAINTメッセージから
	class DcImp_Message : public DcImp{
	public:
		DcImp_Message(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		~DcImp_Message();
	private:
		HWND		m_hwnd;
		PAINTSTRUCT	m_ps;
	};
	//メモリDC
	class DcImp_Memory : public DcImp{
	public:
		DcImp_Memory(const math::CBoxSize& cBoxSize);
		~DcImp_Memory();
	private:
		HBITMAP	m_hbmp;
		HBITMAP	m_hbmpOrg;
	};
public:
	//初期化と終了
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
		//HDC削除
		delete m_pcImp;
		//描画オブジェクト削除
		delete m_pcFont;
	}
	//初期化と終了：実装
	void _BeforeInit()
	{
		m_pcImp = NULL;
		m_hdc = NULL;
		m_pcFont = NULL;
	}
	void _AfterInit();
public:
	// 図形描画
	void FillRect(const math::CRect& rc, COLORREF color);
	void DrawRect(const math::CRect& rc, COLORREF color);
	void DrawLine(const math::CPoint& ptStart, const math::CPoint& ptEnd, COLORREF color);
	// テキスト描画
	void SetTextFont(const wchar_t* szFace, const CPixel& cHeight);
	void SetTextColor(COLORREF color);
	void DrawText(const wchar_t* szText, const math::CPoint& ptPos);
	// イメージ描画
	void DrawImage(const math::CRect& rcDst, const CDib& cDib, const math::CRect& rcSrc);
	void DrawImage(const math::CRect& rcDst, HDC hdc, const math::CRect& rcSrc);
	// 属性取得
	math::CRect	GetRect() const	{ return m_rc; }
	HDC		GetHDC() const	{ return m_hdc; }
private:
	//HDC属性
	DcImp*		m_pcImp;
	HDC			m_hdc;		//m_pcImpからの_GetHDC()参照を保持する。
	math::CRect		m_rc;
	//描画オブジェクト
	CFont*		m_pcFont;	//必要なときだけ生成される
};

