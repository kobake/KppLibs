#pragma once

/*
	���O�X�g���[���B

	�t�@�C���ɏo�͂��邽�߂̕��i�B
	�������[�e�[�g�@�\�t���B
*/
class CLogStream{
public:
	CLogStream(const wchar_t* szFileNameTemplate, int nRotationSize, bool bLogHold);
	~CLogStream();
	void			AddRef()	{ m_nRef++; }								//�Q�ƃJ�E���^��1���₷
	void			Release()	{ m_nRef--; if(m_nRef <= 0)delete this; }	//�Q�ƃJ�E���^��1���炷�B�[���ɂȂ��������B
	void			ReOpen();						// ���O���J�������B�i���[�e�[�g�j
	void			PrintLn(const char* str);		// ���O�Ƀe�L�X�g���o�́B
	const wchar_t*	GetFileNameTemplate() const	{ return m_strFileNameTemplate.c_str(); }
	void			SetRotationSize(int nSize)	{ m_nRotationSize = nSize; }
private:
	int				m_nRef;
	std::wstring	m_strFileNameTemplate;
	std::wstring	m_strCurrentFileName;
	FILE*			m_fp;
	int				m_nRotationSize;
	bool			m_bLogHold;
};
