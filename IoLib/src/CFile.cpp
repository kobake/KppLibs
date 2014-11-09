#include "common/_required.h"
#include "CFile.h"
#include "CFileStream.h"
#include "util/file.h"
using namespace std;

void CFile::ReadFileBytes(std::vector<unsigned char>* pvBuf) const
{
	CFileInputStream in(m_cPath.GetPath(), L"rb");
	//�o�b�t�@�m��
	uint nSize = in.Available();
	pvBuf->resize(nSize);
	//�ǂݎ��
	in.Read(&(*pvBuf)[0], nSize);
}

bool CFile::IsDirectory() const
{
	//GetLogicalDrives�Œ��ׂ�
	if(wcslen(m_cPath.GetPath())==2){
		wstring strDrive = m_cPath.GetDrive();
		if(strDrive.length()>=1){
			int nDrive = towupper(strDrive[0]);
			nDrive-=L'A';
			DWORD dwDrives = ::GetLogicalDrives();
			return (dwDrives & (1<<nDrive))!=0;
		}
	}

	//FindFirstFile�Œ��ׂ�
	WIN32_FIND_DATA fd;
	HANDLE hFind = FindFirstFile(m_cPath.GetPath(), &fd);
	if(hFind==INVALID_HANDLE_VALUE)return false;
	FindClose(hFind);
	return (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)!=0;
}

bool CFile::IsFile() const
{
	WIN32_FIND_DATA fd;
	HANDLE hFind = FindFirstFile(m_cPath.GetPath(), &fd);
	if(hFind == INVALID_HANDLE_VALUE)return false;
	FindClose(hFind);
	return (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0;
}

bool CFile::IsExist() const
{
	return fexist(m_cPath.GetPath());
}



// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                         �e�푮��                            //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

__int64 CFile::GetFileSize() const
{
	return getfsize(m_cPath.GetPath());
}

//�쐬����
time_t CFile::GetCreatedTime() const
{
	struct _stat s;
	if(0==_wstat(m_cPath.GetPath(), &s)){
		return s.st_ctime;
	}
	else{
		return 0;
	}
}

//�A�N�Z�X����
time_t CFile::GetAccessedTime() const
{
	struct _stat s;
	if(0==_wstat(m_cPath.GetPath(), &s)){
		return s.st_atime;
	}
	else{
		return 0;
	}
}

//�X�V����
time_t CFile::GetModifiedTime() const
{
	struct _stat s;
	if(0==_wstat(m_cPath.GetPath(), &s)){
		return s.st_mtime;
	}
	else{
		return 0;
	}
}

#if 0

#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>
#include <direct.h>
#include <errno.h>

#include "CFileInputStream.h"
#include "CFileOutputStream.h"

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                         �쐬�E�j��                           //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
void File::create(int att) //0:�t�@�C�� 1:�t�H���_
{
	if(att==0){
		FILE* fp=fopen(path.GetPath(),"wb");
		fclose(fp);
	}else if(att==1){
		_mkdir(path.GetPath());
	}
}
void File::remove()
{
	int att=getAttribute();
	if(att==0){
		::remove(path.GetPath());
	}else if(att==1){
		rmdir(path.GetPath());
	}
}
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                         �ړ��E�R�s�[                           //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
void File::move(const CFilePath& dst)
{
	int att=getAttribute();
	int ret;
	if(att==0){
		ret=rename(path.GetPath(),dst.GetPath());
		if(ret!=0){
			copy(dst);
			remove();
		}
	}else if(att==1){
		ret=rename(path.GetPath(),dst.GetPath());
	}
}
void File::copy(const CFilePath& dst)
{
	int att=getAttribute();
	if(att==0){
		CFileInputStream in(path.GetPath(),Io::binary);
		CFileOutputStream out(dst.GetPath(),Io::binary);
		try{
			int buf[256];
			while(1){ in.Read(buf,sizeof(buf)); out.write(buf,in.count()); }
		}catch(const Io::EofException&){
		}
	}else if(att==1){
		
	}
}
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                         ���擾                            //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
bool File::exist()
{
	struct _stat s;
	return _stat(path.GetPath(),&s)==0;
}
int File::GetSize()
{
	struct _stat s;
	if(_stat(path.GetPath(),&s)==0){
		return s.st_size; //���t�H���_�̏ꍇ�� 0
	}else{
		return -1; //�Ȃ�
	}
}
int File::getAttribute() //0:�t�@�C�� 1:�t�H���_ -1:�Ȃ�
{
	struct _stat s;
	if(_stat(path.GetPath(),&s)==0){
		if(s.st_mode & _S_IFDIR)
			return 1;
		else
			return 0;
	}else{
		return -1; //�Ȃ�
	}
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           �A�N�Z�X                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
void File::touch()
{
	FILE* fp=fopen(path.GetPath(),"ab");
	if(fp)fclose(fp);
	else
		throw touch_failed();
}


#endif
