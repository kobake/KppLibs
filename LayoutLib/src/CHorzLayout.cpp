#include "common/_required.h"
#include "CHorzLayout.h"
#include "util.h"
#include <GraphicsLib.h>
#include <Cast.h> //WinSystemLib
#include <DebugLib.h>
using namespace std;

//分割バーの幅
static const int SPLIT_WIDTH = 6;

//パートウィンドウのキャプション縦幅 ####本当はWindows設定から取得したほうがいい
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
	//※CSplitBarは随時ウィンドウ自体が移動を繰り返すので、ウィンドウからの相対マウス座標は使い物にならない。
	//  そのため、マウス座標の記録は絶対座標を用いる。
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
		//ドラッグ開始
		::SetCapture(this->GetHWND());
		::GetCursorPos(&old_pt);
		break;
	case WM_LBUTTONUP:
		//ドラッグ終了
		::ReleaseCapture();
		break;
	case WM_MOUSEMOVE:
		//ドラッグ中
		if(::GetCapture()==this->GetHWND()){
			POINT new_pt;
			::GetCursorPos(&new_pt);
			if(new_pt.x==old_pt.x)return 0L;

			//反映
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

	//本体
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

	//キャンバス
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

	//閉じるボタン
	if(bCloseButton){
		m_cCloseButton.Create(L"×", CRect(0,0,0,0), this->GetHWND());
	}
}

LRESULT CPartWindow::OnMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg){
	case WM_DRAWITEM:
		//####できれば完全に CCoolButton へカプセル化したいなぁ。
		{
			DRAWITEMSTRUCT* p = (DRAWITEMSTRUCT*)lParam;
			if(this->m_cCloseButton.ProcessDrawItem(p)){
				return TRUE;
			}
		}
		break;
	case WM_COMMAND:
		//閉じるボタンが押された
		if((HWND)lParam == this->m_cCloseButton.GetHWND()){
			//自分を隠す
			m_pcParentRef->ShowCol(m_nPartIndex, false);
		}
		break;
	case WM_PAINT:
		{
			CDcGraphics g(this->GetHWND(), msg, wParam, lParam);
			if(m_strCaption.length()){
				//キャプション
				g.SetTextFont(L"ＭＳ ゴシック", CPixel(CAPTION_SIZE-10));
				g.SetTextColor(0x000000);
				g.DrawText(m_strCaption.c_str(), CPoint(5,5));
				//キャンバスとの境界線
				g.DrawLine(
					CPoint(0, CAPTION_SIZE-1),
					CPoint(g.GetRect().Width(), CAPTION_SIZE-1),
					::GetSysColor(COLOR_3DSHADOW)
				);
				//上側にも境界線を付ける ###これはPartWindow側でやるべきか、子側にやらせるべきか、悩むところ・・
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
// 初期化と終了
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
void CHorzLayout::Create(
	HWND								hwndParent,
	const std::vector<LayoutPartInfo>&	vPartInfos
)
{
	assert(m_vParts.size()==0);

	//フレーム
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

	//カラムの数だけ子ウィンドウを作る
	for(int i=0;i<(int)vPartInfos.size();i++){
		m_vParts.push_back(new LayoutPart(this, i, vPartInfos[i], this->GetHWND()));
	}

	//カラムの間にサイズ調整バーを作る
	for(int i=0;i<(int)vPartInfos.size()-1;i++){
		CSplitBar* pcSplit = new CSplitBar(this, i);
		pcSplit->Create();
		m_vSplitBars.push_back(pcSplit);
	}

	//フレームを親のサイズに合わせる
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
// インターフェース
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void CHorzLayout::ShowCol(int nIndex, bool bShow)
{
	m_vParts[nIndex]->SetVisible(bShow);
	if(nIndex!=(int)m_vParts.size()-1){
		m_vSplitBars[nIndex]->SetVisible(bShow);
	}

	//サイズ再計算
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
// 実装補助
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
int CHorzLayout::_TryResize(int nSplitIndex, int nOffsetX)
{
	//両側ウィンドウ (※表示されていないものは飛ばす)
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
	if(!pcLeft || !pcRight)return 0; //表示されているパートが見つからない
	CRect rcLeft_Old = pcLeft->GetRect();
	CRect rcRight_Old = pcRight->GetRect();
	

	//計算
	CRect rcLeft = rcLeft_Old;
	CRect rcRight = rcRight_Old;
	rcLeft.OffsetWidth(nOffsetX, X_LEFT);
	rcRight.OffsetWidth(-nOffsetX, X_RIGHT);

	//制限
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


	//反映
	if(rcLeft!=rcLeft_Old || rcRight!=rcRight_Old){
		pcLeft->Move(rcLeft);
		pcRight->Move(rcRight);
		//バーも更新
		CRect rcBar; rcBar.SetLtrb(rcLeft.Right(), 0, rcRight.Left(), rcRight.Height());
		m_vSplitBars[nSplitIndex]->Move(rcBar);
		return rcLeft.Right() - rcLeft_Old.Right();
	}
	else{
		return 0;
	}
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// イベント
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

			//パーツが無い場合は何もしない
			if(m_vParts.size()==0)return 0L;

			//幅リスト -> vSizes
			vector<int> vSizes(m_vParts.size());
			if(m_vParts[0]->GetRect().IsEmpty()){ //初回実行時はデフォルトサイズの配列とする ####visibleがfalseになってもrectがemptyになったりしないように注意
				for(int i=0;i<(int)m_vParts.size();i++){
					vSizes[i] = m_vParts[i]->_GetDefaultSize();
				}
			}
			else{
				for(int i=0;i<(int)m_vParts.size();i++){
					if(m_vParts[i]->IsVisible()){ //※表示されているものだけを保持
						vSizes[i] = m_vParts[i]->GetRect().Width();
					}
					else{
						vSizes[i] = 0;
					}
				}
			}

			//全体の合計幅を計算 -> nSumW
			int nSumW = 0;
			for(int i=0;i<(int)vSizes.size();i++){
				if(m_vParts[i]->IsVisible()){ //※表示されているものだけについて、算出
					if(nSumW > 0)nSumW += SPLIT_WIDTH;
					nSumW += vSizes[i];
				}
			}
			
			//現在のフレーム長との差を計算 -> nGapW
			int nGapW = nFrameW - nSumW;

			//差を分配する (対象：無限最大長) -> vRects
			while(1){
				bool bProvided = false;
				for(int i=0;i<(int)m_vParts.size();i++){
					if(!m_vParts[i]->IsVisible())continue; //※表示されていないものは対象外
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

			//差を分配する (対象：固定最大長)
			while(1){
				bool bProvided = false;
				for(int i=0;i<(int)m_vParts.size();i++){
					if(!m_vParts[i]->IsVisible())continue; //※表示されていないものは対象外
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

			//この時点で振り分けが仕切れないこともあるが、諦めてそのまま進む。
			if(nGapW != 0){
				debugPrint("Warning: サイズ差分を振り分け切れませんでした。残り[%d]\n", nGapW);
			}

			//vSizesに従って再配置
			int x = 0;
			for(int i=0;i<(int)m_vParts.size();i++){
				if(m_vParts[i]->IsVisible()){ //※表示されているものについてのみ反映
					//子ウィンドウ
					LayoutPart* part = m_vParts[i];
					int w = vSizes[i];
					int h = nFrameH;
					part->Move(CRect(x, 0, w, h));
					x += w;
					//子ウィンドウの右のサイズ調整バー
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
