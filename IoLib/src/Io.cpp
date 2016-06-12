#include "_required.h"
#include "Io.h"
#include "file.h"

//!�t�@�C�����̕ύX�B���s�����ꍇ�͗�O�𓊂���B
void Io::rename(
	const wchar_t* oldpath,
	const wchar_t* newpath,
	bool overwrite)
{
	//oldpath�̑��݃`�F�b�N
	if(!fexist(oldpath))throw myexception(L"rename: oldpath[%s]�����݂��܂���",oldpath);

	//�㏑���������ꍇ�A�V�����p�X�ɑ��݂���t�@�C�����폜
	if(overwrite)_wremove(newpath);

	//rename���s
	int ret=::_wrename(oldpath,newpath);
	if(ret!=0){
		throw myexception(L"rename(%s,%s) failed.",oldpath,newpath);
	}
}

