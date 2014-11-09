#pragma once

namespace util{

//�f�B���N�g������
bool dexist(const wchar_t* path);
int dtouch(const wchar_t* _path);

//�����̎擾
__int64 getfsize(const wchar_t* szPath);
int getfpsize(FILE *fp);

//�t�@�C���P�ʂ̑���
bool fexist(const wchar_t* path);
int ftouch(const wchar_t* path);
int fcopy(FILE *fpin,FILE *fpout);            //!<�X�g���[������X�g���[���փR�s�[���s��
int fcopy(const wchar_t* from,const wchar_t* to); //!<�t�@�C������t�@�C���փR�s�[���s��
void fbackup(const wchar_t* fpath); //!<�t�@�C���̃o�b�N�A�b�v���쐬����B�o�b�N�A�b�v�t�@�C�����́u(���̃t�@�C����).(�C���f�b�N�X)�v�Ƃ���B

//���e�̕ҏW
int fstrreplace(FILE *fpin,FILE *fpout,const wchar_t* from,const wchar_t* to);
int fstrreplace(const wchar_t* path,const wchar_t* from,const wchar_t* to);
int fstrreplace(const wchar_t* pathfrom,const wchar_t* pathto,const wchar_t* from,const wchar_t* to);

//�������ϊ�
void* file_alloc(const wchar_t* path,const wchar_t* mode);
void file_free(void* p);
/*
inline int fstrreplace(FILE *fpin,FILE *fpout,const string& from,const string& to)
{
	return fstrreplace(fpin,fpout,from.c_str(),to.c_str());
}
inline int fstrreplace(const string& path,const string& from,const string& to)
{
	return fstrreplace(path.c_str(),from.c_str(),to.c_str());
}
inline int fstrreplace(const string& pathfrom,const string& pathto,const string& from,const string& to)
{
	return fstrreplace(pathfrom.c_str(),pathto.c_str(),from.c_str(),to.c_str());
}
*/

} //namespace util

using namespace util;
