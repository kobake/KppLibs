#pragma once

#include <vector>
#include <window/CWindow.h> //WinSystemLib
#include <string>
#include "CCoolButton.h"

class CHorzLayout;

//1�̃Z�������`����\���́B
//nDefaultSize��-1���w�肷��ƁA�K���ȃf�t�H���g�l�ɕϊ������
//nMinSize��-1���w�肷��ƁA�K���ȃf�t�H���g�l�ɕϊ������
//nMaxSize��-1���w�肷��ƁA�ő�l�����͖����i�������j�ƂȂ�
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
				nDefaultSize = nMinSize; //�ŏ��l�Ɠ��l
			}
			else{
				nDefaultSize = (nMinSize + nMaxSize) / 2; //�ŏ��l�ƍő�l�̒��Ԓl
			}
		}

		if(nMaxSize==-1){
			nMaxSize = INT_MAX;
		}
	}
};

//��؂�p�E�B���h�E
class CSplitBar : public CWindow{
public:
	CSplitBar(CHorzLayout* pcParent, int nSplitIndex) : m_pcParentRef(pcParent), m_nSplitIndex(nSplitIndex) { }
	void Create();
	LRESULT OnMessage(UINT msg, WPARAM wParam, LPARAM lParam);
private:
	CHorzLayout*	m_pcParentRef;
	int				m_nSplitIndex;
};

//�p�[�g�E�B���h�E
class CPartWindow : public CWindow{
public:
	CPartWindow(CHorzLayout* pcParent, int nPartIndex) : m_pcParentRef(pcParent), m_nPartIndex(nPartIndex), m_hwndCanvasRef(NULL) { }
	void Create(HWND hwndParent, const wchar_t* szCaption, bool bCloseButton);
	LRESULT OnMessage(UINT msg, WPARAM wParam, LPARAM lParam);
	void _AttachChild(HWND hwndChild);
private:
	CHorzLayout*	m_pcParentRef;
	int				m_nPartIndex;
	HWND			m_hwndCanvasRef;	// �T�C�Y����шʒu�𒲐�����ΏہB //CWindow			m_cCanvas;
	CCoolButton		m_cCloseButton;
	std::wstring	m_strCaption;
};

//���C�A�E�g�{��
class CHorzLayout : public CWindow{
public:
	//�^
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

		//�T�C�Y���� ####LayoutPartInfo���ł���������
		CRect _AdjustRect(const CRect& _rc, Align align)
		{
			CRect rc = _rc;
			if(rc.Width() < m_info.nMinSize){ //�T�C�Y��������
				int r = m_info.nMinSize - rc.Width();
				rc.OffsetWidth(r, align);
			}
			if(rc.Width() > m_info.nMaxSize){ //�T�C�Y�傫����
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

		//�J�v�Z�����\��F�����A�j���A�ړ��i������RECT��ێ��j�ARECT����
	};
public:
	//�������ƏI��
	~CHorzLayout();
	void Create(
		HWND								hwndParent,
		const std::vector<LayoutPartInfo>&	vPartInfos
	);
	//�C���^�[�t�F�[�X
	CWindow* GetColWindow(int nIndex){ return m_vParts[nIndex]->GetWindow(); }
	void ShowCol(int nIndex, bool bShow);
	bool IsColVisible(int nIndex) const;
	void AttachToCol(int nIndex, HWND hwndChild);
	//�C�x���g
	LRESULT OnMessage(UINT msg, WPARAM wParam, LPARAM lParam);
public:
	//�����⏕
	int _TryResize(int nSplitIndex, int nOffsetX); //!< ���ۂɓ���������Ԃ�
private:
	std::vector<LayoutPart*>	m_vParts;
	std::vector<CSplitBar*>		m_vSplitBars;
};
