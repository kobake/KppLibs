#pragma once

enum ELogTarget{
	LOG_TARGET_NONE		= 0,
	LOG_TARGET_STDOUT	= (1<<0),
	LOG_TARGET_STDERR	= (1<<1),
	LOG_TARGET_FILE		= (1<<2),
	LOG_TARGET_DEBUGGER	= (1<<3),
};

inline ELogTarget operator | (ELogTarget e1, ELogTarget e2)
{
	return (ELogTarget)((int)e1 | (int)e2);
}

/*
	���O�^�C�v�B

	���O��ޖ������O�ɏo�͂�����A
	���O��ɂ���ăR���\�[���o�͂�ON/OFF������A�o�̓t�@�C����ʌɂ�����B
*/
class CLogType{
public:
	// ������
	CLogType();
	~CLogType();

	// �ݒ� //
	void		SetName(const char* szName)	{ m_strName = szName; }
	void		SetFile(const wchar_t* szFileNameTemplate, bool bLogHold);
	void		SetTarget(ELogTarget eTarget)	{ m_eTarget = eTarget; }
	void		SetRotationSize(int nSize);

	// �m�F //
	const char*		GetName() const				{ return m_strName.c_str(); }
	const wchar_t*	GetFile() const				{ return m_pcStreamRef?m_pcStreamRef->GetFileNameTemplate():L""; }
	int				GetRotationSize() const		{ return m_nRotationSize; }
	ELogTarget		GetTarget() const			{ return m_eTarget; }

	// �g�p //
	void	Print(const char* str);

private:
	//�R�s�[�֎~
	CLogType(const CLogType&);
	CLogType& operator = (const CLogType&);

private:
	std::string		m_strName;
	ELogTarget		m_eTarget;
	CLogStream*		m_pcStreamRef;
	int				m_nRotationSize;
};
