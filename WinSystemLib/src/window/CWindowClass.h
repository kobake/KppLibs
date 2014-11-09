#pragma once

#include <windows.h> //HINSTANCE

class CWindow;

class CWindowClass{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CWindowClass();
	CWindowClass(HINSTANCE _hInst,const wchar_t* _class_name);
	~CWindowClass();

	//�I�v�V�����ݒ�
	void SetMenuName(const wchar_t* pMenuName);
	void SetIcon(HICON hIcon)					{ m_hIcon = hIcon; }

	//�o�^
	bool Regist(WNDPROC WndProc) const; //�N���X��V�����o�^������true, ���ɓo�^�ς݂�������false, ����ȊO�̃G���[�ł͗�O���΂�

	//�擾
	bool IsValid() const{ return GetName()!=NULL; }
	HINSTANCE GetModule() const{ return m_hInst; }
	const wchar_t* GetName() const{ return m_szClassName; }
	const wchar_t* GetMenuName() const
	{
		return
			(m_typeMenuName==0)?NULL:
			(m_typeMenuName==1)?m_szMenuName:
			(*(const wchar_t**)m_szMenuName);
	}

private:
	//�K�{
	HINSTANCE	m_hInst;
	wchar_t		m_szClassName[32];

	//�I�v�V����
	int			m_typeMenuName;		//0:NULL, 1:������, 2:INTRESOURCE
	wchar_t		m_szMenuName[32];	//�󕶎���̏ꍇ��NULL����
	HICON		m_hIcon;
};

