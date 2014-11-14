#include "common/_required.h"
#include "CFileShell.h"
#include "../CWinSystem.h"
#include <IoLib.h> // CFilePath
#include <shlobj.h>
#include "../CClipboard.h"
#include <StringLib.h>
#include <ShellAPI.h>

using namespace std;

void _FilesToClipboard(
	HWND						hwnd,
	const vector<CFilePathWin>&	vFiles,
	int							nFlag		//DROPEFFECT_MOVE:切り取り DROPEFFECT_COPY:コピー
);
bool _ClipboardToFiles(
	HWND					hwnd,
	vector<CFilePathWin>*	pvFiles,
	DWORD*					pdwFlag		//DROPEFFECT_MOVE:切り取り DROPEFFECT_COPY:コピー
);
bool _CreateShortCut(
	const wchar_t*	szShortcutFile,		//生成されるショートカットファイルのパス
	const wchar_t*	szSourcePath,		//リンク先
	const wchar_t*	szArgs,				//引数
	const wchar_t*	szWorkingDir,		//実行される場所
	const wchar_t*	szDescription		//説明
);
static void _InvokeProperties(
	const vector<CFilePathWin>& vFiles
);


//複数文字列をヌル区切りで連結したバッファ（終端にはヌルが2つ入る）
class CStringNullChain{
public:
	CStringNullChain()
	{
		m_vBuf.push_back(0);
		m_vBuf.push_back(0);
	}
	void Push(const wchar_t* szText)
	{
		const wchar_t* pEnd = wcschr(szText,L'\0')+1;
		if(m_vBuf.size()==2)m_vBuf.pop_back();
		m_vBuf.pop_back();							//リスト終端ヌルを一時的に削除
		m_vBuf.insert(m_vBuf.end(),szText,pEnd);	//終端ヌルを含めたデータを追加
		m_vBuf.push_back(0);						//リスト終端ヌルを追加し直し
	}
	void Push(const vector<wstring>& vFiles)
	{
		for(int i=0;i<(int)vFiles.size();i++){
			Push(vFiles[i].c_str());
		}
	}
	const wchar_t* GetBuffer() const
	{
		return &m_vBuf[0];
	}
private:
	std::vector<wchar_t> m_vBuf;
};


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                        操作                           //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void CFileShell::Open()
{
	ShellExecute(
		CWinSystem::GetMainWindow(),
		L"open",
		m_cPath.GetPath(),
		L"",
		L"",
		SW_SHOW
	);
}

/*
void CFileShell::Delete()
{
	CStringNullChain buf; buf.Push(m_cPath.c_str());
	SHFILEOPSTRUCT op;
	memset(&op, 0, sizeof(op));
	op.hwnd = CWinSystem::GetMainWindow();
	op.wFunc = FO_DELETE;
	op.pFrom = buf.GetBuffer();
	op.fFlags = FOF_ALLOWUNDO;
	int nRet = ::SHFileOperation(&op);
}
*/
void CFileShell::Paste()
{
	std::vector<CFilePathWin> vPathes;
	DWORD dwFlag = 0;
	bool b = _ClipboardToFiles(CWinSystem::GetMainWindow(), &vPathes, &dwFlag);
	if(b){
		if(dwFlag & DROPEFFECT_COPY){
			CFilesShell(vPathes).CopyToDir(m_cPath);
		}
		else if(dwFlag & DROPEFFECT_MOVE){
			CFilesShell(vPathes).MoveToDir(m_cPath);
		}
		else if(dwFlag & DROPEFFECT_LINK){
			CFilesShell(vPathes).CreateShortcutToDir(m_cPath);
		}
	}
}

void CFileShell::RenameTo(const CFilePathWin& cNewPath)
{
	if(wcscmp(m_cPath.GetPath(), cNewPath.GetPath())==0)return; //名前が変わらない場合は何もしない
//	cNewPath.SetTitle(szNewName);

	CStringNullChain bufFrom; bufFrom.Push(m_cPath.GetPath());
	CStringNullChain bufTo; bufTo.Push(cNewPath.GetPath());
	SHFILEOPSTRUCT op;
	memset(&op, 0, sizeof(op));
	op.hwnd = CWinSystem::GetMainWindow();
	op.wFunc = FO_RENAME;
	op.pFrom = bufFrom.GetBuffer();
	op.pTo = bufTo.GetBuffer();
	op.fFlags = FOF_ALLOWUNDO;
	int nRet = ::SHFileOperation(&op);
}

/*
void CFileShell::Cut()
{
	vector<wstring> vFiles;
	vFiles.push_back(this->m_cPath);
	_FilesToClipboard(CWinSystem::GetMainWindow(), vFiles, DROPEFFECT_MOVE);
}
*/

/*
void CFileShell::Copy()
{
	vector<wstring> vFiles;
	vFiles.push_back(this->m_cPath);
	_FilesToClipboard(CWinSystem::GetMainWindow(), vFiles, DROPEFFECT_COPY);
}
*/

void CFileShell::Property()
{
	HWND hwnd = CWinSystem::GetMainWindow();
	SHELLEXECUTEINFO sh;
	memset(&sh, 0, sizeof(sh));
	sh.cbSize = sizeof(sh);
	sh.fMask = SEE_MASK_NOCLOSEPROCESS | SEE_MASK_INVOKEIDLIST | SEE_MASK_FLAG_NO_UI;
	sh.lpVerb = L"Properties";
	sh.hwnd = hwnd;
	sh.lpParameters = NULL;
	sh.lpDirectory = NULL;
	sh.lpIDList = NULL;
	sh.lpFile = this->m_cPath.GetPath();
	int nRet = ::ShellExecuteEx(&sh);
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                    操作（複数）                       //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

// 確認ダイアログを出さずにゴミ箱に移動 //
void CFilesShell::Delete(bool bConfirmDialog)
{
	if(m_vFiles.size()==0)return;

	//バッファ構築 ###できれば効率化したい
	CStringNullChain buf;
	for(int i=0;i<(int)m_vFiles.size();i++)buf.Push(m_vFiles[i].GetPath());

	SHFILEOPSTRUCT op;
	memset(&op, 0, sizeof(op));
	op.hwnd = CWinSystem::GetMainWindow();
	op.wFunc = FO_DELETE;
	op.pFrom = buf.GetBuffer();
	op.fFlags = FOF_ALLOWUNDO | (bConfirmDialog?0:FOF_NOCONFIRMATION);
	int nRet = ::SHFileOperation(&op);
}

void CFilesShell::Cut()
{
	if(m_vFiles.size()==0)return;

	_FilesToClipboard(CWinSystem::GetMainWindow(), m_vFiles, DROPEFFECT_MOVE);
}

void CFilesShell::Copy()
{
	if(m_vFiles.size()==0)return;

	_FilesToClipboard(CWinSystem::GetMainWindow(), m_vFiles, DROPEFFECT_COPY);
}

void CFilesShell::MoveToDir(const CFilePathWin& cDir)
{
	if(m_vFiles.size()==0)return;

	CStringNullChain bufFrom;
	for(int i = 0; i < (int)m_vFiles.size(); i++){
		bufFrom.Push(m_vFiles[i].GetPath());
	}

	CStringNullChain bufTo;
	bufTo.Push(cDir.GetPath());

	SHFILEOPSTRUCT op;
	memset(&op, 0, sizeof(op));
	op.hwnd   = CWinSystem::GetMainWindow();
	op.wFunc  = FO_MOVE;
	op.pFrom  = bufFrom.GetBuffer();
	op.pTo    = bufTo.GetBuffer();
	op.fFlags = FOF_ALLOWUNDO;
	int nRet = ::SHFileOperation(&op);
}

void CFilesShell::Property()
{
	_InvokeProperties(m_vFiles);
}

#include <IoLib.h> // CFilePath

void CFilesShell::CreateShortcutToDir(const CFilePathWin& cDir)
{
	if(m_vFiles.size()==0)return;

	for(int i=0;i<(int)m_vFiles.size();i++){
		CFilePathUnix cSrcFile = m_vFiles[i].GetPath(); // UnixPathに変換 //
		CFilePathUnix cDstFile(cDir.GetPath(), ((wstring)cSrcFile.GetTitle() + L".lnk").c_str());
		bool bRet = _CreateShortCut(
			cDstFile.GetPath(),
			cSrcFile.GetPath(),
			L"",
			cSrcFile.GetDir(),
			L""
		);
	}
}

void CFilesShell::CopyToDir(const CFilePathWin& cDir)
{
	if(m_vFiles.size()==0)return;

	CStringNullChain bufFrom;
	for(int i = 0; i < (int)m_vFiles.size(); i++){
		bufFrom.Push(m_vFiles[i].GetPath());
	}

	CStringNullChain bufTo;
	bufTo.Push(cDir.GetPath());

	SHFILEOPSTRUCT op;
	memset(&op, 0, sizeof(op));
	op.hwnd   = CWinSystem::GetMainWindow();
	op.wFunc  = FO_COPY;
	op.pFrom  = bufFrom.GetBuffer();
	op.pTo    = bufTo.GetBuffer();
	op.fFlags = FOF_ALLOWUNDO;
	int nRet = ::SHFileOperation(&op);
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                      実装補助                         //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void _FilesToClipboard(
	HWND						hwnd,
	const vector<CFilePathWin>&	vFiles,
	int							nFlag		//DROPEFFECT_MOVE:切り取り DROPEFFECT_COPY:コピー
)
{
	//CF_HDROPを構築
	HGLOBAL hDrop = NULL;
	{
		//必要なメモリ量
		int nSize = sizeof(DROPFILES);
		for(int i=0;i<(int)vFiles.size();i++){
			nSize+=(int)(wcslen(vFiles[i].GetPath())+1)*sizeof(wchar_t);
		}
		nSize+=sizeof(wchar_t);

		//メモリ確保
		hDrop = GlobalAlloc(GHND, nSize);

		//データ格納
		char* p = static_cast<char*>(GlobalLock(hDrop));
		{
			//DROPFILES部分
			DROPFILES* pDrop = (DROPFILES*)p;
			memset(pDrop, 0, sizeof(*pDrop));
			pDrop->pFiles = sizeof(*pDrop); //ファイル名が格納されているブロックへのオフセット
			pDrop->fWide = TRUE;
			p+=sizeof(*pDrop);

			//ファイル名部分
			for(int i=0; i < (int)vFiles.size(); i++) {
				wcscpy((wchar_t*)p, vFiles[i].GetPath());
				p += (wcslen(vFiles[i].GetPath())+1)*sizeof(wchar_t);
			}
			*p++ = 0;
			*p++ = 0;
		}

		//データ格納完了
		GlobalUnlock(hDrop);
	}

	//Preferred DropEffectを構築
	HGLOBAL hDropEffect = NULL;
	{
		hDropEffect = GlobalAlloc(GHND, sizeof(DWORD));
		DWORD* pdw = (DWORD*)GlobalLock(hDropEffect);
		*pdw = nFlag;
		GlobalUnlock(hDropEffect);
	}

	//クリップボードにデータをセット
	UINT CF_DROPEFFECT = RegisterClipboardFormat(CFSTR_PREFERREDDROPEFFECT);
	CClipboard clip(hwnd);
	clip.Empty();
	clip.SetData(CF_HDROP, hDrop);
	clip.SetData(CF_DROPEFFECT, hDropEffect);
	clip.Close();
}

bool _ClipboardToFiles(
	HWND					hwnd,
	vector<CFilePathWin>*	pvFiles,
	DWORD*					pdwFlag		//DROPEFFECT_MOVE:切り取り DROPEFFECT_COPY:コピー
)
{
	pvFiles->clear();
	*pdwFlag = 0;

	CClipboard clip(hwnd);
	if(!clip)return false;

	//CF_HDROP -> pvFiles
	{
		HDROP hDrop = (HDROP)clip.GetData(CF_HDROP);
		if(!hDrop)return false;
		int nFiles = DragQueryFile(hDrop, -1, L"", 0);

		wchar_t buf[1024];
		for(int i=0;i<nFiles;i++){
			DragQueryFile(hDrop, i, buf, _countof(buf));
			pvFiles->push_back(buf);
		}
	}
	
	//CF_DROPEFFECT -> pdwFlag
	*pdwFlag = DROPEFFECT_COPY;
	{
		UINT CF_DROPEFFECT = RegisterClipboardFormat(CFSTR_PREFERREDDROPEFFECT);
	
		HGLOBAL hDropEffect = clip.GetData(CF_DROPEFFECT);
		if(!hDropEffect)return true; //CF_DROPEFFECTは無くても良い
		DWORD* pdw = (DWORD*)GlobalLock(hDropEffect);
		*pdwFlag = *pdw;
		GlobalUnlock(hDropEffect);
		/*
		if(*pdwFlag & DROPEFFECT_COPY){
			wprintf(L"effect = copy\n");
		}
		else if(*pdwFlag & DROPEFFECT_MOVE){
			wprintf(L"effect = move\n");
		}
		else if(*pdwFlag & DROPEFFECT_LINK){
			wprintf(L"effect = shortcut\n");
		}
		*/
	}
	return true;
}


//参考：http://park6.wakwak.com/~wmasa/prog/srclib/0008.htm
bool _CreateShortCut(
	const wchar_t*	szShortcutFile,		//生成されるショートカットファイルのパス
	const wchar_t*	szSourcePath,		//リンク先
	const wchar_t*	szArgs,				//引数
	const wchar_t*	szWorkingDir,		//実行される場所
	const wchar_t*	szDescription		//説明
)
{
	HRESULT hRet;

	CoInitialize(NULL);

	//IShellLinkの作成
	IShellLink* pShellLink;
	hRet = CoCreateInstance(
		CLSID_ShellLink,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IShellLink,
		(void **)&pShellLink
	);
	if(SUCCEEDED(hRet)){
		//IPersistFileの取得
		IPersistFile* pPersistFile;
		hRet = pShellLink->QueryInterface(
			IID_IPersistFile,
			(void **)&pPersistFile
		);
		if(SUCCEEDED(hRet)){
			pShellLink->SetPath(szSourcePath);
			pShellLink->SetArguments(szArgs);
			pShellLink->SetWorkingDirectory(szWorkingDir);
			pShellLink->SetDescription(szDescription);

			hRet = pPersistFile->Save(szShortcutFile,TRUE);

			pPersistFile->Release();
		}
		pShellLink->Release();
	}
	CoUninitialize();
	return SUCCEEDED(hRet)!=0;
}


#include "CShellFolder.h"
#include "CContextMenu.h"

static void _InvokeProperties(
	const vector<CFilePathWin>& vFiles
)
{
	if(vFiles.size()==0)return;

	//vFiles[0]のルート -> strRoot
	wstring strRoot;
	int nRootLen = 0; //ルート長。各子アイテムのパスオフセットとして用いる。必ずしもstrRoot.length()とは一致しないことに注意。
	{
		const wchar_t* szPath = vFiles[0].GetPath();
		const wchar_t* p = szPath;
		if(*p==L'\\'){
			p++;
			if(*p!=L'\\')return; //失敗：ネットワークではなかった
			p++;
			while(*p!=L'\\')p++;
			strRoot.assign(szPath, p); //※終端の「\」は含めない
			nRootLen = (int)strRoot.length()+1;
		}
		else if((*p>=L'A' && *p<='Z') || (*p>=L'a' && *p<=L'z')){
			p++;
			if(*p!=L':')return; //失敗：ドライブではなかった
			p++;
			if(*p!=L'\\')return; //失敗：ドライブではなかった
			strRoot.assign(szPath, p + 1); //※終端の「\」も含める
			nRootLen = (int)strRoot.length(); //「X:\」の長さ //
		}
		else{
			return; //失敗：不明なルート
		}
	}

	//すべてのルートが同一であることを確認
	for(int i=0;i<(int)vFiles.size();i++){
		if(wcsncmp(vFiles[i].GetPath(), strRoot.c_str(), strRoot.length())!=0){
			//失敗：異なるルートが存在する
			strRoot = L"";
			nRootLen = 0;
			break;
		}
	}
	if(strRoot.length()==0)return;	//失敗：異なるルートが存在する

	//ルートのオブジェクトを構築
	CShellFolder cRoot(strRoot.c_str());

	//アイテム群のITEMIDLISTを構築
	vector<CItemIdList> vItems;
	for(int i=0;i<(int)vFiles.size();i++){
		vItems.push_back(
			cRoot.CreateItemIDList(vFiles[i].GetPath() + nRootLen)
		);
	}

	//メニュー
	CContextMenu cMenu(cRoot, vItems);

	//Invokeする
	CMINVOKECOMMANDINFO cmi;
	memset(&cmi, 0, sizeof(cmi));
	cmi.cbSize = sizeof(cmi);
	cmi.fMask = SEE_MASK_INVOKEIDLIST;
	cmi.lpVerb = "Properties";
	cmi.lpParameters = NULL;
	cmi.lpDirectory = NULL;//"D:/_tmp";
	cmi.nShow = SW_SHOW;
	cmi.dwHotKey = 0;
	cmi.hIcon = NULL;
	HRESULT hRet = cMenu->InvokeCommand(&cmi);
}
