#include "common/_required.h"
#include "CHorzLayout.h"
#include "util.h"
#include <GraphicsLib.h>
#include <Cast.h> //WinSystemLib
#include <DebugLib.h>
using namespace std;

//�����o�[�̕�
static const int SPLIT_WIDTH = 6;

//�p�[�g�E�B���h�E�̃L���v�V�����c�� ####�{����Windows�ݒ肩��擾�����ق�������
static const int CAPTION_SIZE = 25;

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// CSplitBar
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
void CSplitBar::Create()
{
	CWindow::Create(
		SWindowInfo(
			CWindowClass(c2lib::GetInstance(), L"SplitForHorz"),
			0,
			WS_CHILD | WS_VISIBLE,
			L"",
			CRect(0,0,0,0),
			m_pcParentRef->GetHWND(),
			NULL
		)
	);
}

LRESULT CSplitBar::OnMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
	//��CSplitBar�͐����E�B���h�E���̂��ړ����J��Ԃ��̂ŁA�E�B���h�E����̑��΃}�E�X���W�͎g�����ɂȂ�Ȃ��B
	//  ���̂��߁A�}�E�X���W�̋L�^�͐�΍��W��p����B
	static POINT old_pt;

	switch(msg){
	case WM_PAINT:
		{
			CDcGraphics g(this->GetHWND(), msg, wParam, lParam);
			CRect rc = g.GetRect();
			g.FillRect(rc,::GetSysColor(COLOR_BTNFACE));
			rc.OffsetTop(-1);
			rc.OffsetBottom(1);
			g.DrawRect(rc,::GetSysColor(COLOR_3DSHADOW));
		}
		break;
	case WM_SETCURSOR:
		::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
		break;
	case WM_LBUTTONDOWN:
		//�h���b�O�J�n
		::SetCapture(this->GetHWND());
		::GetCursorPos(&old_pt);
		break;
	case WM_LBUTTONUP:
		//�h���b�O�I��
		::ReleaseCapture();
		break;
	case WM_MOUSEMOVE:
		//�h���b�O��
		if(::GetCapture()==this->GetHWND()){
			POINT new_pt;
			::GetCursorPos(&new_pt);
			if(new_pt.x==old_pt.x)return 0L;

			//���f
			int gap_x = new_pt.x - old_pt.x;
			debugPrint("old_x = %d, new_x = %d, gap_x = %d\n", old_pt.x, new_pt.x, gap_x);
			int nResult = m_pcParentRef->_TryResize(m_nSplitIndex, gap_x);
			new_pt.x = old_pt.x + nResult;

			old_pt = new_pt;
		}
		break;
	default:
		return __super::OnMessage(msg, wParam, lParam);
	}
	return 0L;
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// CPartWindow
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
void CPartWindow::Create(HWND hwndParent, const wchar_t* szCaption, bool bCloseButton)
{
	m_strCaption = szCaption;

	//�{��
	CWindow::Create(
		SWindowInfo(
			CWindowClass(c2lib::GetInstance(), L"HorzLayoutCol"),
			0,
			WS_VISIBLE | WS_CHILD,
			L"",
			CRect(0,0,0,0),
			hwndParent,
			NULL
		)
	);

	//�L�����o�X
	/*
	m_cCanvas.Create(
		SWindowInfo(
			CWindowClass(c2lib::GetInstance(), L"LayoutCanvas"),
			0,
			WS_VISIBLE | WS_CHILD,
			L"",
			CRect(0,0,0,0),
			this->GetHWND(),
			NULL
		)
	);
	*/

	//����{�^��
	if(bCloseButton){
		m_cCloseButton.Create(L"�~", CRect(0,0,0,0), this->GetHWND());
	}
}

LRESULT CPartWindow::OnMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg){
	case WM_DRAWITEM:
		//####�ł���Ί��S�� CCoolButton �փJ�v�Z�����������Ȃ��B
		{
			DRAWITEMSTRUCT* p = (DRAWITEMSTRUCT*)lParam;
			if(this->m_cCloseButton.ProcessDrawItem(p)){
				return TRUE;
			}
		}
		break;
	case WM_COMMAND:
		//����{�^���������ꂽ
		if((HWND)lParam == this->m_cCloseButton.GetHWND()){
			//�������B��
			m_pcParentRef->ShowCol(m_nPartIndex, false);
		}
		break;
	case WM_PAINT:
		{
			CDcGraphics g(this->GetHWND(), msg, wParam, lParam);
			if(m_strCaption.length()){
				//�L���v�V����
				g.SetTextFont(L"�l�r �S�V�b�N", CPixel(CAPTION_SIZE-10));
				g.SetTextColor(0x000000);
				g.DrawText(m_strCaption.c_str(), CPoint(5,5));
				//�L�����o�X�Ƃ̋��E��
				g.DrawLine(
					CPoint(0, CAPTION_SIZE-1),
					CPoint(g.GetRect().Width(), CAPTION_SIZE-1),
					::GetSysColor(COLOR_3DSHADOW)
				);
				//�㑤�ɂ����E����t���� ###�����PartWindow���ł��ׂ����A�q���ɂ�点��ׂ����A�Y�ނƂ���E�E
				g.DrawLine(
					CPoint(0, 0),
					CPoint(g.GetRect().Width(), 0),
					::GetSysColor(COLOR_3DSHADOW)
				);
			}
		}
		break;
	case WM_SIZE:
		{
			int w = LOWORD(lParam);
			int h = HIWORD(lParam);
			if(m_cCloseButton.GetHWND()){
				m_cCloseButton.Move(CRect(w - 3 - (CAPTION_SIZE - 3*2), 3, CAPTION_SIZE - 3*2, CAPTION_SIZE - 3*2));
			}
			int y = 0;
			if(m_strCaption.length() || m_cCloseButton.GetHWND()){
				y = CAPTION_SIZE;
			}
			if(m_hwndCanvasRef){
				::MoveWindow(m_hwndCanvasRef, 0, y, w, h - y, TRUE);
			}
		}
		break;
	default:
		return __super::OnMessage(msg, wParam, lParam);
	}
	return 0L;
}

void CPartWindow::_AttachChild(HWND hwndChild)
{
	m_hwndCanvasRef = hwndChild;
	::SetParent(m_hwndCanvasRef, this->GetHWND());
	this->PostSizeMessage();
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// �������ƏI��
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
void CHorzLayout::Create(
	HWND								hwndParent,
	const std::vector<LayoutPartInfo>&	vPartInfos
)
{
	assert(m_vParts.size()==0);

	//�t���[��
	CWindow::Create(
		SWindowInfo(
			CWindowClass(c2lib::GetInstance(), L"HorzLayout"),
			0,
			WS_VISIBLE | WS_CHILD,
			L"",
			CRect(0,0,0,0),
			hwndParent,
			NULL
		)
	);

	//�J�����̐������q�E�B���h�E�����
	for(int i=0;i<(int)vPartInfos.size();i++){
		m_vParts.push_back(new LayoutPart(this, i, vPartInfos[i], this->GetHWND()));
	}

	//�J�����̊ԂɃT�C�Y�����o�[�����
	for(int i=0;i<(int)vPartInfos.size()-1;i++){
		CSplitBar* pcSplit = new CSplitBar(this, i);
		pcSplit->Create();
		m_vSplitBars.push_back(pcSplit);
	}

	//�t���[����e�̃T�C�Y�ɍ��킹��
	RECT rc;
	::GetClientRect(hwndParent, &rc);
	this->Move(RECT2Rect(rc));
}

CHorzLayout::~CHorzLayout()
{
	for(int i=0;i<(int)m_vParts.size();i++){
		delete m_vParts[i];
	}
	m_vParts.clear();

	for(int i=0;i<(int)m_vSplitBars.size();i++){
		delete m_vSplitBars[i];
	}
	m_vSplitBars.clear();
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// �C���^�[�t�F�[�X
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void CHorzLayout::ShowCol(int nIndex, bool bShow)
{
	m_vParts[nIndex]->SetVisible(bShow);
	if(nIndex!=(int)m_vParts.size()-1){
		m_vSplitBars[nIndex]->SetVisible(bShow);
	}

	//�T�C�Y�Čv�Z
	this->PostSizeMessage();
}

bool CHorzLayout::IsColVisible(int nIndex) const
{
	return this->m_vParts[nIndex]->IsVisible();
}

void CHorzLayout::AttachToCol(int nIndex, HWND hwndChild)
{
	m_vParts[nIndex]->_AttachChild(hwndChild);
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// �����⏕
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
int CHorzLayout::_TryResize(int nSplitIndex, int nOffsetX)
{
	//�����E�B���h�E (���\������Ă��Ȃ����͔̂�΂�)
	LayoutPart* pcLeft = NULL;
	for(int i=nSplitIndex;i>=0;i--){
		if(m_vParts[i]->IsVisible()){
			pcLeft = m_vParts[i];
			break;
		}
	}
	LayoutPart* pcRight = NULL;
	for(int i=nSplitIndex+1;i<(int)m_vParts.size();i++){
		if(m_vParts[i]->IsVisible()){
			pcRight = m_vParts[i];
			break;
		}
	}
	if(!pcLeft || !pcRight)return 0; //�\������Ă���p�[�g��������Ȃ�
	CRect rcLeft_Old = pcLeft->GetRect();
	CRect rcRight_Old = pcRight->GetRect();
	

	//�v�Z
	CRect rcLeft = rcLeft_Old;
	CRect rcRight = rcRight_Old;
	rcLeft.OffsetWidth(nOffsetX, X_LEFT);
	rcRight.OffsetWidth(-nOffsetX, X_RIGHT);

	//����
	CRect tmp;
	tmp = pcLeft->_AdjustRect(rcLeft, X_LEFT);
	if(tmp != rcLeft){
		rcRight.OffsetWidth(-(tmp.Right() - rcLeft.Right()), X_RIGHT);
		rcLeft = tmp;
	}
	tmp = pcRight->_AdjustRect(rcRight, X_RIGHT);
	if(tmp != rcRight){
		rcLeft.OffsetWidth(-(rcRight.Left() - tmp.Left()), X_LEFT);
		rcRight = tmp;
	}


	//���f
	if(rcLeft!=rcLeft_Old || rcRight!=rcRight_Old){
		pcLeft->Move(rcLeft);
		pcRight->Move(rcRight);
		//�o�[���X�V
		CRect rcBar; rcBar.SetLtrb(rcLeft.Right(), 0, rcRight.Left(), rcRight.Height());
		m_vSplitBars[nSplitIndex]->Move(rcBar);
		return rcLeft.Right() - rcLeft_Old.Right();
	}
	else{
		return 0;
	}
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// �C�x���g
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

LRESULT CHorzLayout::OnMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg){
	case WM_CREATE:
		break;
	case WM_SIZE:
		{
//			CLockWindowUpdate lock(this->GetHWND());

			int nFrameW = LOWORD(lParam);
			int nFrameH = HIWORD(lParam);

			//�p�[�c�������ꍇ�͉������Ȃ�
			if(m_vParts.size()==0)return 0L;

			//�����X�g -> vSizes
			vector<int> vSizes(m_vParts.size());
			if(m_vParts[0]->GetRect().IsEmpty()){ //������s���̓f�t�H���g�T�C�Y�̔z��Ƃ��� ####visible��false�ɂȂ��Ă�rect��empty�ɂȂ����肵�Ȃ��悤�ɒ���
				for(int i=0;i<(int)m_vParts.size();i++){
					vSizes[i] = m_vParts[i]->_GetDefaultSize();
				}
			}
			else{
				for(int i=0;i<(int)m_vParts.size();i++){
					if(m_vParts[i]->IsVisible()){ //���\������Ă�����̂�����ێ�
						vSizes[i] = m_vParts[i]->GetRect().Width();
					}
					else{
						vSizes[i] = 0;
					}
				}
			}

			//�S�̂̍��v�����v�Z -> nSumW
			int nSumW = 0;
			for(int i=0;i<(int)vSizes.size();i++){
				if(m_vParts[i]->IsVisible()){ //���\������Ă�����̂����ɂ��āA�Z�o
					if(nSumW > 0)nSumW += SPLIT_WIDTH;
					nSumW += vSizes[i];
				}
			}
			
			//���݂̃t���[�����Ƃ̍����v�Z -> nGapW
			int nGapW = nFrameW - nSumW;

			//���𕪔z���� (�ΏہF�����ő咷) -> vRects
			while(1){
				bool bProvided = false;
				for(int i=0;i<(int)m_vParts.size();i++){
					if(!m_vParts[i]->IsVisible())continue; //���\������Ă��Ȃ����̂͑ΏۊO
					if(m_vParts[i]->_GetMaxSize() == INT_MAX){
						if(nGapW > 0){
							if(vSizes[i] < m_vParts[i]->_GetMaxSize()){
								vSizes[i]++;
								nGapW--;
								bProvided = true;
							}
						}
						else if(nGapW < 0){
							if(vSizes[i] > m_vParts[i]->_GetMinSize()){
								vSizes[i]--;
								nGapW++;
								bProvided = true;
							}
						}
						else{
							break;
						}
					}
				}
				if(!bProvided)break;
			}

			//���𕪔z���� (�ΏہF�Œ�ő咷)
			while(1){
				bool bProvided = false;
				for(int i=0;i<(int)m_vParts.size();i++){
					if(!m_vParts[i]->IsVisible())continue; //���\������Ă��Ȃ����̂͑ΏۊO
					if(m_vParts[i]->_GetMaxSize() != INT_MAX){
						if(nGapW > 0){
							if(vSizes[i] < m_vParts[i]->_GetMaxSize()){
								vSizes[i]++;
								nGapW--;
								bProvided = true;
							}
						}
						else if(nGapW < 0){
							if(vSizes[i] > m_vParts[i]->_GetMinSize()){
								vSizes[i]--;
								nGapW++;
								bProvided = true;
							}
						}
						else{
							break;
						}
					}
				}
				if(!bProvided)break;
			}

			//���̎��_�ŐU�蕪�����d�؂�Ȃ����Ƃ����邪�A���߂Ă��̂܂ܐi�ށB
			if(nGapW != 0){
				debugPrint("Warning: �T�C�Y������U�蕪���؂�܂���ł����B�c��[%d]\n", nGapW);
			}

			//vSizes�ɏ]���čĔz�u
			int x = 0;
			for(int i=0;i<(int)m_vParts.size();i++){
				if(m_vParts[i]->IsVisible()){ //���\������Ă�����̂ɂ��Ă̂ݔ��f
					//�q�E�B���h�E
					LayoutPart* part = m_vParts[i];
					int w = vSizes[i];
					int h = nFrameH;
					part->Move(CRect(x, 0, w, h));
					x += w;
					//�q�E�B���h�E�̉E�̃T�C�Y�����o�[
					if(i!=(int)m_vParts.size()-1){
						CWindow* sizing = m_vSplitBars[i];
						w = SPLIT_WIDTH;
						sizing->Move(CRect(x, 0, w, h));
						x += w;
					}
				}
			}
		}
		break;
	default:
		return __super::OnMessage(msg, wParam, lParam);
	}
	return 0L;
}
