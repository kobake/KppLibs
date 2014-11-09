#include "_required.h"
#include "CFilePath.h"
#include <StringLib.h>
using namespace util;
using namespace std;

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//               �R���X�g���N�^�E�f�X�g���N�^                  //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
CFilePath_Base::CFilePath_Base(const wchar_t* fpath, wchar_t cDelimiter)
: m_cDelimiter(cDelimiter)
{
	m_delimiter=NULL;
	m_extpos=NULL;
	SetPath(fpath);
}
CFilePath_Base::CFilePath_Base(const wchar_t* fdir,const wchar_t* ftitle, wchar_t cDelimiter)
: m_cDelimiter(cDelimiter)
{
	m_delimiter=NULL;
	m_extpos=NULL;
	SetPath(fdir,ftitle);
}

CFilePath_Base::~CFilePath_Base()
{
}

void CFilePath_Base::SetDelimiter(wchar_t cDelimiter)
{
	if(m_cDelimiter != cDelimiter){
		wcsreplace(m_path, m_cDelimiter, cDelimiter);
		m_cDelimiter = cDelimiter;
	}
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           ���                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
void CFilePath_Base::Assign(const CFilePath_Base& f)
{
	//�R�s�[����
	int copylen=(int)wcslen(f.m_path)+1;

	//�R�s�[
	wmemcpy(m_path,f.m_path,copylen);
	if(f.m_delimiter)m_delimiter=m_path+(f.m_delimiter-f.m_path); else m_delimiter = NULL;
	if(f.m_extpos)m_extpos=m_path+(f.m_extpos-f.m_path); else m_extpos = NULL;
	
	//����
	this->m_cDelimiter = f.m_cDelimiter;
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           �ݒ�                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void CFilePath_Base::SetPath(const wchar_t* fpath)
{
	wcssafecpy(m_path,fpath,countof(m_path));
	_UpdateDelimiter();
}

void CFilePath_Base::SetPath(const wchar_t* fdir,const wchar_t* ftitle)
{
	wcssafecpy(m_path,fdir,countof(m_path));
	if(*m_path!=L'\0' && *(wcschr(m_path,L'\0')-1)!=m_cDelimiter)wcscat_c(m_path,m_cDelimiter);
	wcssafecat(m_path,ftitle,countof(m_path));
	_UpdateDelimiter();
}

void CFilePath_Base::SetDir(const wchar_t* fdir)
{
	wstring title = GetTitle();
	SetPath(fdir, title.c_str());
}

void CFilePath_Base::SetTitle(const wchar_t* ftitle)
{
	if(m_delimiter!=NULL){
		*m_delimiter = m_cDelimiter;
		wcssafecpy(m_delimiter+1,ftitle,countof(m_path)-(m_delimiter-m_path+1));
		_UpdateDelimiter();
	}
	else{
		wcssafecpy(m_path,ftitle,countof(m_path));
		_UpdateDelimiter();
	}
}

void CFilePath_Base::SetExt(const wchar_t* ext)
{
	if(m_extpos!=NULL){
		wcssafecpy(m_extpos+1,ext,countof(m_path)-(m_extpos-m_path+1));
	}
	else{
		*m_delimiter = m_cDelimiter;
		wcssafecat(m_path,L".",countof(m_path));
		wcssafecat(m_path,ext,countof(m_path));
		_UpdateDelimiter();
	}
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           �擾                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
const wchar_t* CFilePath_Base::GetPath() const
{
	return m_path;
}

const wchar_t* CFilePath_Base::GetDir() const
{
	//�L���b�V���X�V
	if(m_strDirCache.length()==0){
		if(m_delimiter){
			m_strDirCache.assign(m_path, m_delimiter);
		}
		else{
			//�󔒂̂܂�
		}
	}
	return m_strDirCache.c_str();
}
const wchar_t* CFilePath_Base::GetTitle() const
{
	if(m_delimiter!=NULL){
		return m_delimiter+1;
	}else{
		return m_path;
	}
}
const wchar_t* CFilePath_Base::GetExt() const
{
	if(m_extpos!=NULL){
		return m_extpos;
	}else{
		return L"";
	}
}

std::wstring CFilePath_Base::GetTitleWithoutExt() const
{
	wstring str = GetTitle();
	size_t n = str.find(L'.');
	if(n==wstring::npos)return str;
	str = str.substr(0, n);
	return str;
}

std::wstring CFilePath_Base::GetDrive() const
{
	if(wcslen(m_path)>=2 && m_path[1]==L':'){
		int nDrive = m_path[0];
		if(nDrive>=L'a' && nDrive<=L'z' || L'A' && nDrive<=L'Z'){
			wchar_t ret[] = {nDrive, L':', L'\0'};
			return ret;
		}
	}
	return L"";
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                         ��؂��                            //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void CFilePath_Base::_UpdateDelimiter()
{
	//�p�X���́u..�v����сu.�v������
	{
		wstring tmp = fextract2(m_path, m_cDelimiter); //######���ׂɒ���
		wcscpy(m_path, tmp.c_str());
	}

	//###�K�v���Ȃ��E�E�E
//	wcsreplace(m_path,L'/',L'\\');

	//m_delimiter,m_extpos�X�V
	m_delimiter=wcsrchr(m_path, m_cDelimiter);
	if(m_delimiter!=NULL){
		m_extpos=wcschr(m_delimiter, L'.');
	}
	else{
		m_extpos=wcschr(m_path,L'.');
	}

	//�L���b�V���N���A
	m_strDirCache = L"";
}

