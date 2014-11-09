#pragma once

class CModuleInfo;
class CLogType;

/*
	�ėp�̃��O��
*/
extern CLogType _TRACE;
extern CLogType _INFO;
extern CLogType _WARN;
extern CLogType _ERROR;
extern CLogType _FATAL;
extern CLogType _QUERY;

class CLogManager : public TSingleton<CLogManager>{
public:
	CLogManager()
	{
		m_bInitialized = false;
	}
	void Init();
	void Init(
		const CModuleInfo&	cModuleInfo,
		const wchar_t*		szLogPath	= L"$exedir/log/$exename_$yyyy$mm$dd_$HH$MM$SS_$N.log",
		bool				bLogHold	= true	// ���O���I�[�v�������ςȂ��ɂ��邩 //
	);
	void InitNoLog();
	bool IsInitialized() const	{ return m_bInitialized; }
private:
	bool m_bInitialized;
};
