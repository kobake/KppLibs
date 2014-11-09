#include "_required.h"
#include "CCommandLine_Old.h"
#include <DebugLib.h>
using namespace std;

inline bool is_sp(wchar_t c)
{
	return c==' ' || c=='\t';
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//               �R���X�g���N�^�E�f�X�g���N�^                  //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

CCommandLine_Old::CCommandLine_Old(int argc,const wchar_t* argv[])
{
	for(int i=0;i<argc;i++){
		args.push_back(argv[i]);
	}
}

CCommandLine_Old::CCommandLine_Old(int argc,wchar_t* argv[])
{
	for(int i=0;i<argc;i++){
		args.push_back(argv[i]);
	}
}

CCommandLine_Old::CCommandLine_Old(const wchar_t* str)
{
	//���
	const wchar_t* p=str;
	const wchar_t* begin;
	const wchar_t* end;
	while(p){
		//�󔒓ǂݔ�΂�
		while(is_sp(*p))p++;

		//�I�[�Ȃ�I��
		if(*p==L'\0')break;

		//�g�[�N���J�n
		begin=p;

		//�g�[�N��
		if(*p=='"'){
			p++;
			while(*p!=L'"' && *p!=L'\0')p++;
			if(*p==L'"')p++;
		}
		else{
			while(!is_sp(*p) && *p!=L'\0')p++;
		}

		//�g�[�N���I��
		end=p;

		//�N�H�[�e�[�V��������
		if(*begin=='"')begin++;
		if(begin<end && *(end-1)=='"')end--;

		//���X�g�ǉ�
		args.push_back(wstring(begin,end));
	}
	
	wstring s=str;
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           �擾                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

size_t CCommandLine_Old::size() const
{
	return args.size();
}
const wchar_t* CCommandLine_Old::at(size_t i) const
{
	return args[i].c_str();
}
const wchar_t* CCommandLine_Old::operator [] (size_t i) const
{
	return args[i].c_str();
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           ����                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

inline bool isopt(wchar_t c)
{
	return c==L'-' || c==L'/';
}

//! �P���ɁAstr�����݂��邩�ǂ����𒲂ׂ�B���݂��Ă����true�B�啶���A�������͋�ʂ��Ȃ��B
bool CCommandLine_Old::existRaw(const wchar_t* str) const
{
	for(int i=0;i<(int)args.size();i++){
		if(wcsicmp(args[i].c_str(),str)==0){
			return true;
		}
	}
	return false;
}

bool CCommandLine_Old::ExistFlag(const wchar_t* opt) const
{
	return FindFlag(opt)>=0;
}

//! "-opt" �܂��� "/opt" �������B�啶���A�������͋�ʂ��Ȃ�
int CCommandLine_Old::FindFlag(const wchar_t* opt) const
{
	for(int i=0;i<(int)args.size();i++){
		const wchar_t* arg=args[i].c_str();
		if(isopt(arg[0]) && wcsicmp(&arg[1],opt)==0){
			return i;
		}
	}
	return -1;
}


const wchar_t* CCommandLine_Old::_getpopOption(const wchar_t* opt,const wchar_t* def,bool to_remove)
{
	const wchar_t* ret=def;
	int found_i=-1;
	int found_n=0;

	size_t nopt=wcslen(opt);
	for(int i=0;i<(int)args.size();i++){
		const wchar_t* arg=args[i].c_str();
		if(isopt(arg[0]) && wcsnicmp(&arg[1],opt,nopt)==0){
			//"="��؂�̒l
			if(arg[1+nopt]=='='){
				ret=&arg[1+nopt+1];
				found_i=i;
				found_n=1;
				goto lbl_ret;
			}
			//" "��؂�̒l
			else if(arg[1+nopt]=='\0'){
				if(i+1<(int)args.size()){
					ret=args[i+1].c_str();
					found_i=i;
					found_n=2;
					goto lbl_ret;
				}
				else{
					ret=L"";
					found_i=i;
					found_n=1;
					goto lbl_ret;
				}
			}
		}
	}
lbl_ret:
	if(to_remove && found_i!=-1){
		//�߂�l��static�ȗ̈�ɑޔ�
		static wstring sret;
		sret=ret;
		//�Y�����ڂ�����
		for(int i=0;i<found_n;i++){
			remove(found_i);
		}
		//static�ȗ̈��Ԃ�
		return sret.c_str();
	}else{
		return ret;
	}
}


//!�I�v�V�����l�̎擾
/*!
"-opt=?" �܂��� "/opt=?" �� "?" ���擾�B
"=" �̑���� " " ���󂯕t����B
"-opt" �����݂��Ȃ���� def ��Ԃ��B
"-opt" �����݂��邪�A"?"�����݂��Ȃ��ꍇ�A"" ��Ԃ��B
*/
const wchar_t* CCommandLine_Old::GetFlagValue(const wchar_t* szFlagName,const wchar_t* szDefaultValue) const
{
	return const_cast<CCommandLine_Old*>(this)->_getpopOption(szFlagName,szDefaultValue,false);
}

//!�I�v�V�����l�̎擾�B�����B
/*!
GetFlagValue�̓���ɉ����A�����������ڂ���菜���B
*/
const wchar_t* CCommandLine_Old::PopFlag(const wchar_t* szFlagName,const wchar_t* szDefaultValue)
{
	return _getpopOption(szFlagName,szDefaultValue,true);
}



// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           �ҏW                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//! �R�}���h���C���o�b�t�@������������
void CCommandLine_Old::Assign(size_t i,const wchar_t* str)
{
#if _MSC_VER >= 1400
	assert(i<args.size());
#endif
	args[i]=str;
}

//! �w��C���f�b�N�X�̍��ڂ���菜��
void CCommandLine_Old::remove(size_t i)
{
#if _MSC_VER >= 1400
	assert(i<args.size());
#endif
	vector<wstring>::iterator p=args.begin();
	while(i>0){ i--; p++; }
	args.erase(p);
}

//! �Ō���Ɉ�����ǉ�����
void CCommandLine_Old::push_back(const wchar_t* str)
{
	args.push_back(str);
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           �z��                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

const wchar_t* const* CCommandLine_Old::getArgs(size_t begin_index) const
{
	for(int i=0;i<(int)args.size();i++){
		args_buf[i]=args[i].c_str();
	}
	args_buf[args.size()]=NULL;
	return args_buf;
}
