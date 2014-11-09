#pragma once

#include <vector>
#include <string>

//!�R�}���h���C����̓N���X
class CCommandLine_Old{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CCommandLine_Old(const wchar_t* str);
	CCommandLine_Old(int argc,const wchar_t* argv[]);
	CCommandLine_Old(int argc,wchar_t* argv[]);

	//�擾
	size_t size() const;                       //!< �����̐�
	const wchar_t* at(size_t i) const;           //!< �������擾
	const wchar_t* operator [] (size_t i) const; //!< �������擾

	//main�� �擾
	size_t argc() const{ return size(); }
	const wchar_t** argv() const{ return (const wchar_t**)getArgs(0); } 

	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
	//                           ����                              //
	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

	bool existRaw(const wchar_t* str) const;          //!< �P���ɁAstr�����݂��邩�ǂ����𒲂ׂ�B���݂��Ă����true�B�啶���A�������͋�ʂ��Ȃ��B
	bool ExistFlag(const wchar_t* opt) const;       //!< "-opt" �܂��� "/opt" �������B�啶���A�������͋�ʂ��Ȃ�
	int FindFlag(const wchar_t* opt) const;         //!< "-opt" �܂��� "/opt" �������B�啶���A�������͋�ʂ��Ȃ�

	const wchar_t* _getpopOption(const wchar_t* opt,const wchar_t* def,bool to_remove);

	/*
		!�I�v�V�����l�̎擾
		"-opt=?" �܂��� "/opt=?" �� "?" ���擾�B
		"=" �̑���� " " ���󂯕t����B
		"-opt" �����݂��Ȃ���� NULL ��Ԃ��B
		"-opt" �����݂��邪�A"?"�����݂��Ȃ��ꍇ�A"" ��Ԃ��B
	*/
	const wchar_t* GetFlagValue(const wchar_t* szFlagName,const wchar_t* szDefaultValue) const;

	/*!
		�I�v�V�����l�̎擾�B�����B
		GetFlagValue�̓���ɉ����A�����������ڂ���菜���B
	*/
	const wchar_t* PopFlag(const wchar_t* szFlagName,const wchar_t* szDefaultValue);

	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
	//                           �ҏW                              //
	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
	void Assign(size_t i,const wchar_t* str);         //!< �R�}���h���C���o�b�t�@������������
	void remove(size_t i);                          //!< �w��C���f�b�N�X�̍��ڂ���菜��
	void push_back(const wchar_t* str);               //!< �Ō���Ɉ�����ǉ�����

	//�z��
	const wchar_t* const* getArgs(size_t begin_index) const; //!< �I�[�� NULL �̔z���Ԃ�
private:
	std::vector<std::wstring> args;
	mutable const wchar_t* args_buf[64];
};
