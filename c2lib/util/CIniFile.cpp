#include "common/_required.h"
#include "CIniFile.h"
#include <StringLib.h>
#include "util/file.h"
#include <IoLib.h> //CFileStream.h
#include <comutil.h>
#include <windows.h>
using namespace std;
using namespace util;

// ================================================================================ //
// CIniSection
// ================================================================================ //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// �������ƏI��
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
CIniSection::CIniSection(const wchar_t* _fpath,const wchar_t* _section_name)
: m_strFilePath(_fpath), m_strSectionName(_section_name)
{
}

CIniSection::~CIniSection()
{
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// �ǎ�
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//!��������擾
wstring CIniSection::GetString(const wchar_t* szKey,const wchar_t* def) const
{
	static wchar_t tmp[_MAX_PATH];
	GetPrivateProfileString(m_strSectionName.c_str(),szKey,def,tmp,countof(tmp),m_strFilePath.c_str());
	return tmp;
}

//!��������擾�B���ϐ�%�`%��u������
wstring CIniSection::GetStringEx(const wchar_t* szKey,const wchar_t* def) const
{
	static wchar_t tmp[_MAX_PATH];
	static wchar_t tmp2[_MAX_PATH];
	GetPrivateProfileString(m_strSectionName.c_str(),szKey,def,tmp,countof(tmp),m_strFilePath.c_str());
	ExpandEnvironmentStrings(tmp,tmp2,_MAX_PATH);
	return tmp2;
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// ����
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//!�������ݒ�
void CIniSection::WriteString(const wchar_t* szKey,const wchar_t* szValue)
{
	ftouch(m_strFilePath.c_str());
	WritePrivateProfileString(m_strSectionName.c_str(), szKey, szValue, m_strFilePath.c_str());
}



// ================================================================================ //
// CIniFile
// ================================================================================ //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// �������ƏI��
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

CIniFile::CIniFile(const wchar_t* _fpath)
: m_cFilePath(_fpath)
{
	//�t���p�X�łȂ��ꍇ�A�J�����g�f�B���N�g���܂��̓V�X�e���f�B���N�g������t�@�C����T���A
	//�t���p�X�����蓖�Ă�
	if(wcswcs(_fpath,L":")==NULL){
		wchar_t buf[_MAX_PATH];
		wchar_t* delimiter;
		DWORD ret=SearchPath(NULL,_fpath,NULL,countof(buf),buf,&delimiter);
		if(ret!=0){
			m_cFilePath.SetPath(buf);
		}
	}
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                  �Z�N�V�����ւ̃A�N�Z�X                     //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//�����ӁFANSI�̂ݎ�t
int CIniFile::GetSectionCount() const
{
	try{
		CFileInputStream in(m_cFilePath.GetPath(),L"rt");
		//�s�̐擪��"["���J�E���g����
		int cnt=0;
		int c;
		bool linehead=true;
		while((c=in.get())!=EOF){
			if(c=='\n'){ linehead=true; continue; }
			if(linehead){
				if(c=='[')cnt++;
				linehead=false;
			}
		}
		return cnt;
	}
	catch(Io::OpenException){
		return 0;
	}
}

//�����ӁFANSI�̂ݎ�t
CIniSection CIniFile::GetSectionByIndex(int iSectionIndex) const
{
	CFileInputStream in(m_cFilePath.GetPath(),L"rt");
	//�s�̐擪��"["���J�E���g���AiSectionIndex�ɒB����܂ő�������
	int cnt=0;
	int c;
	bool linehead=true;
	while((c=in.get())!=EOF){
		if(c=='\n'){ linehead=true; continue; }
		if(linehead){
			if(c=='[')cnt++;
			linehead=false;

			if(cnt-1==iSectionIndex)goto found;
		}
	}
	throw myexception(L"�Z�N�V����[%d]��������܂���", iSectionIndex);

found:
	//�Z�N�V��������ǂݎ��
	vector<char> buf;
	while(1){
		c=in.get();
		if(c==']' || c==EOF)break;
		if(c=='\n')throw wexception(L"�s����ini�t�@�C���ł�");
		buf.push_back(c);
	}
	buf.push_back('\0');
	_bstr_t section=&buf[0];

	return CIniSection(m_cFilePath.GetPath(),section);
}

CIniSection CIniFile::GetSectionByName(const wchar_t* szSectionName) const
{
	return CIniSection(m_cFilePath.GetPath(), szSectionName);
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           �ǎ�                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

wstring CIniFile::GetString(const wchar_t* szSection,const wchar_t* szKey,const wchar_t* def)
{
	static wchar_t tmp[_MAX_PATH];
	GetPrivateProfileString(szSection, szKey, def, tmp, countof(tmp), m_cFilePath.GetPath());
	return tmp;
}

wstring CIniFile::GetStringEx(const wchar_t* szSection,const wchar_t* szKey,const wchar_t* def)
{
	static wchar_t tmp[_MAX_PATH];
	static wchar_t tmp2[_MAX_PATH];
	GetPrivateProfileString(szSection, szKey, def, tmp, countof(tmp), m_cFilePath.GetPath());
	ExpandEnvironmentStrings(tmp, tmp2, _MAX_PATH);
	return tmp2;
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           ����                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void CIniFile::WriteString(const wchar_t* szSection,const wchar_t* szKey,const wchar_t* szValue)
{
	ftouch(m_cFilePath.GetPath());
	WritePrivateProfileString(szSection, szKey, szValue, m_cFilePath.GetPath());
}
