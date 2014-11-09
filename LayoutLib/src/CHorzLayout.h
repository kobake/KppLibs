#pragma once

#include <vector>
#include <window/CWindow.h> //WinSystemLib
#include <string>
#include "CCoolButton.h"

class CHorzLayout;

//1個のセル情報を定義する構造体。
//nDefaultSizeに-1を指定すると、適当なデフォルト値に変換される
//nMinSizeに-1を指定すると、適当なデフォルト値に変換される
//nMaxSizeに-1を指定すると、最大値制限は無効（無制限）となる
struct LayoutPartInfo{
	int		nDefaultSize;
	int		nMinSize;
	int		nMaxSize;
	wstring	strCaption;
	bool	bCloseButton;

	LayoutPartInfo()
	: nDefaultSize(0), nMinSize(0), nMaxSize(0), strCaption(L"")
	{
	}
	LayoutPartInfo(int _nMinSize, int _nDefaultSize, int _nMaxSize)
	: nDefaultSize(_nDefaultSize), nMinSize(_nMinSize), nMaxSize(_nMaxSize), bCloseButton(false)
	{
		if(nMinSize==-1)nMinSize = 10;

		if(nDefaultSize==-1){
			if(nMaxSize==-1){
				nDefaultSize = nMinSize; //最小値と同値
			}
			else{
				nDefaultSize = (nMinSize + nMaxSize) / 2; //最小値と最大値の中間値
			}
		}

		if(nMaxSize==-1){
			nMaxSize = INT_MAX;
		}
	}
};

//区切り用ウィンドウ
class CSplitBar : public CWindow{
public:
	CSplitBar(CHorzLayout* pcParent, int nSplitIndex) : m_pcParentRef(pcParent), m_nSplitIndex(nSplitIndex) { }
	void Create();
	LRESULT OnMessage(UINT msg, WPARAM wParam, LPARAM lParam);
private:
	CHorzLayout*	m_pcParentRef;
	int				m_nSplitIndex;
};

//パートウィンドウ
class CPartWindow : public CWindow{
public:
	CPartWindow(CHorzLayout* pcParent, int nPartIndex) : m_pcParentRef(pcParent), m_nPartIndex(nPartIndex), m_hwndCanvasRef(NULL) { }
	void Create(HWND hwndParent, const wchar_t* szCaption, bool bCloseButton);
	LRESULT OnMessage(UINT msg, WPARAM wParam, LPARAM lParam);
	void _AttachChild(HWND hwndChild);
private:
	CHorzLayout*	m_pcParentRef;
	int				m_nPartIndex;
	HWND			m_hwndCanvasRef;	// サイズおよび位置を調整する対象。 //CWindow			m_cCanvas;
	CCoolButton		m_cCloseButton;
	std::wstring	m_strCaption;
};

//レイアウト本体
class CHorzLayout : public CWindow{
public:
	//型
	class LayoutPart{
	public:
		LayoutPart(CHorzLayout* pcParent, int nPartIndex, const LayoutPartInfo& info, HWND hwndParent)
		: m_info(info), m_bVisible(true)
		{
			m_window = new CPartWindow(pcParent, nPartIndex);
			m_window->Create(hwndParent, info.strCaption.c_str(), info.bCloseButton);
		}
		~LayoutPart()
		{
			delete m_window;
		}
		int GetCurrentWidth() const{ return GetRect().Width(); }
		CWindow* GetWindow(){ return m_window; }
		const CRect& GetRect() const
		{
			if(m_rc.IsEmpty()){
				m_window->GetWindowRect_OnParent(&m_rc);
			}
			return m_rc;
		}
		void Move(const CRect& rc)
		{
			m_rc = rc;
			m_window->Move(rc);
		}
		bool IsVisible() const
		{
			return m_bVisible;
		}
		void SetVisible(bool b)
		{
			m_bVisible = b;
			m_window->SetVisible(b);
		}
		void _AttachChild(HWND hwndChild)
		{
			m_window->_AttachChild(hwndChild);
		}

		//サイズ調整 ####LayoutPartInfo側でもいいかも
		CRect _AdjustRect(const CRect& _rc, Align align)
		{
			CRect rc = _rc;
			if(rc.Width() < m_info.nMinSize){ //サイズ小さすぎ
				int r = m_info.nMinSize - rc.Width();
				rc.OffsetWidth(r, align);
			}
			if(rc.Width() > m_info.nMaxSize){ //サイズ大きすぎ
				int r = rc.Width() - m_info.nMaxSize;
				rc.OffsetWidth(-r, align);
			}
			return rc;
		}
		int _GetMaxSize() const
		{
			return m_info.nMaxSize;
		}
		int _GetMinSize() const
		{
			return m_info.nMinSize;
		}
		int _GetDefaultSize() const
		{
			return m_info.nDefaultSize;
		}
	private:
		LayoutPartInfo	m_info;
		CPartWindow*	m_window;
		mutable CRect	m_rc;
		bool			m_bVisible;

		//カプセル化予定：生成、破棄、移動（同時にRECTを保持）、RECT情報提供
	};
public:
	//初期化と終了
	~CHorzLayout();
	void Create(
		HWND								hwndParent,
		const std::vector<LayoutPartInfo>&	vPartInfos
	);
	//インターフェース
	CWindow* GetColWindow(int nIndex){ return m_vParts[nIndex]->GetWindow(); }
	void ShowCol(int nIndex, bool bShow);
	bool IsColVisible(int nIndex) const;
	void AttachToCol(int nIndex, HWND hwndChild);
	//イベント
	LRESULT OnMessage(UINT msg, WPARAM wParam, LPARAM lParam);
public:
	//実装補助
	int _TryResize(int nSplitIndex, int nOffsetX); //!< 実際に動いた幅を返す
private:
	std::vector<LayoutPart*>	m_vParts;
	std::vector<CSplitBar*>		m_vSplitBars;
};
