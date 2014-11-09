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

	//�o�^
	if(g_map[hwnd].wndprocOld==NULL){
		g_map[hwnd].wndprocOld = (WNDPROC)(intptr_t)::SetWindowLong(hwnd, GWL_WNDPROC, (LONG)(intptr_t)S_HookWndProc);
	}
	g_map[hwnd].vSubClasses.push_back_unique(this);
}

void CWindowSubclass::UnsetSubclassWindow()
{
	if(!m_hwnd)return;

	//����
	g_map[m_hwnd].vSubClasses.erase_value(this);
	if(g_map[m_hwnd].vSubClasses.size()==0){
		::SetWindowLong(m_hwnd, GWL_WNDPROC, (LONG)(intptr_t)g_map[m_hwnd].wndprocOld);
		g_map.erase(m_hwnd);
	}

	m_hwnd = NULL;
}

LRESULT CALLBACK CWindowSubclass::S_HookWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//WindowInfo�̃C���X�^���X���擾
	const WindowInfo* pInfo = NULL;
	map<HWND, WindowInfo>::const_iterator p = g_map.find(hwnd);
	if(p!=g_map.end()){
		pInfo = &p->second;
	}

	//�C���X�^���X���擾�ł��Ȃ������ꍇ�̓f�t�H���g����
	if(!pInfo){
		return ::DefWindowProc(hwnd, msg, wParam, lParam);
	}

	//�{���̃E�B���h�E�v���V�[�W��
	WNDPROC wndprocOld = pInfo->wndprocOld;

	//���N���X�ɂ�郁�b�Z�[�W�����i�������{���j
	LRESULT lresult = 0L;
	bool bProcessed = false;
	for(int i=0;i<(int)pInfo->vSubClasses.size();i++){
		bProcessed = pInfo->vSubClasses[i]->HandleMessage(hwnd, msg, wParam, lParam, &lresult);
		if(bProcessed)break;
	}

	//WM_DESTROY���ɂ͊֘A�t�����O��
	if(msg==WM_DESTROY){
		vector_ex<CWindowSubclass*> vSubClasses = pInfo->vSubClasses;
		for(int i=0;i<(int)vSubClasses.size();i++){
			vSubClasses[i]->UnsetSubclassWindow();
		}
	}

	//�N���X�ɂ�郁�b�Z�[�W�����̌���
	if(bProcessed){
		return lresult;
	}

	//�N���X�ɂ�郁�b�Z�[�W�������s���Ȃ������ꍇ�̓f�t�H���g����
	return ::CallWindowProc(wndprocOld, hwnd, msg, wParam, lParam);
}


