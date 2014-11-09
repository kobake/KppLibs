#pragma once

/*
	ログストリーム。

	ファイルに出力するための部品。
	自動ローテート機能付き。
*/
class CLogStream{
public:
	CLogStream(const wchar_t* szFileNameTemplate, int nRotationSize, bool bLogHold);
	~CLogStream();
	void			AddRef()	{ m_nRef++; }								//参照カウンタを1増やす
	void			Release()	{ m_nRef--; if(m_nRef <= 0)delete this; }	//参照カウンタを1減らす。ゼロになったら解放。
	void			ReOpen();						// ログを開き直す。（ローテート）
	void			PrintLn(const char* str);		// ログにテキストを出力。
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
