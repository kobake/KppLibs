#include "common/_required.h"
#include "CWindowSubclass.h"
#include <map>
#include <vector>
#include "util/vector_ex.h"
using namespace std;

struct WindowInfo{
	WNDPROC						wndprocOld;
	vector_ex<CWindowSubclass*>	vSubClasses;

	WindowInfo()
	: wndprocOld(NULL)
	{
	}
};
static map<HWND, WindowInfo> g_map;

CWindowSubclass::CWindowSubclass()
: m_hwnd(NULL)
{
}

CWindowSubclass::~CWindowSubclass()
{
}

void CWindowSubclass::SetSubclassWindow(HWND hwnd)
{
	if(m_hwnd)return;

	m_hwnd = hwnd;

	//登録
	if(g_map[hwnd].wndprocOld==NULL){
		g_map[hwnd].wndprocOld = (WNDPROC)(intptr_t)::SetWindowLong(hwnd, GWL_WNDPROC, (LONG)(intptr_t)S_HookWndProc);
	}
	g_map[hwnd].vSubClasses.push_back_unique(this);
}

void CWindowSubclass::UnsetSubclassWindow()
{
	if(!m_hwnd)return;

	//解除
	g_map[m_hwnd].vSubClasses.erase_value(this);
	if(g_map[m_hwnd].vSubClasses.size()==0){
		::SetWindowLong(m_hwnd, GWL_WNDPROC, (LONG)(intptr_t)g_map[m_hwnd].wndprocOld);
		g_map.erase(m_hwnd);
	}

	m_hwnd = NULL;
}

LRESULT CALLBACK CWindowSubclass::S_HookWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//WindowInfoのインスタンスを取得
	const WindowInfo* pInfo = NULL;
	map<HWND, WindowInfo>::const_iterator p = g_map.find(hwnd);
	if(p!=g_map.end()){
		pInfo = &p->second;
	}

	//インスタンスが取得できなかった場合はデフォルト処理
	if(!pInfo){
		return ::DefWindowProc(hwnd, msg, wParam, lParam);
	}

	//本来のウィンドウプロシージャ
	WNDPROC wndprocOld = pInfo->wndprocOld;

	//☆クラスによるメッセージ処理（ここが本命）
	LRESULT lresult = 0L;
	bool bProcessed = false;
	for(int i=0;i<(int)pInfo->vSubClasses.size();i++){
		bProcessed = pInfo->vSubClasses[i]->HandleMessage(hwnd, msg, wParam, lParam, &lresult);
		if(bProcessed)break;
	}

	//WM_DESTROY時には関連付けを外す
	if(msg==WM_DESTROY){
		vector_ex<CWindowSubclass*> vSubClasses = pInfo->vSubClasses;
		for(int i=0;i<(int)vSubClasses.size();i++){
			vSubClasses[i]->UnsetSubclassWindow();
		}
	}

	//クラスによるメッセージ処理の結果
	if(bProcessed){
		return lresult;
	}

	//クラスによるメッセージ処理が行われなかった場合はデフォルト処理
	return ::CallWindowProc(wndprocOld, hwnd, msg, wParam, lParam);
}


