#include "common/_required.h"
#include "../../ThreadLib/src/CCriticalSection.h"
#include "CFtp2.h"
#include <queue>
#include <time.h>
#include <StringLib.h>
using namespace std;

//c1,c2,c3の中で一番先に見つかった場所を返す。どれも見つからなかったらNULLを返す。
const char* strchr3(const char* str, char c1, char c2, char c3)
{
	const char* p = str;
	while(1){
		if(*p==c1 || *p==c2 || *p==c3)return p;
		if(!*p)break;
		p++;
	}
	return NULL;
}

class CLineQueue{
public:
	CLineQueue()
	: m_bBackHalf(false)
	{
	}
	void Push(const char* str)
	{
		CRITICAL(m_cCritical);

		//1行毎に格納
		const char* p = str;
		const char* pBegin = p;
		while(1){
			//※空行は無視する
			while(*p=='\r' || *p=='\n')p++;
			if(!*p)break;
			//単語の終端を探す
			const char* q = strchr3(p, '\r', '\n', '\0');
			//単語格納
			string tmp(p,q);
			if(m_bBackHalf){
				//終端要素が中途半端なので、そこに繋げる形で格納
				m_queueLines.back() += tmp;
			}
			else{
				//通常格納
				m_queueLines.push(tmp);
			}
			if(!*q)m_bBackHalf = true; //\0で終わっている場合は中途半端な可能性がある
			else m_bBackHalf = false;
			//次
			p = q;
		}
	}
	int Peek() const
	{
		CRITICAL(m_cCritical);
		return (int)m_queueLines.size();
	}
	string Pop()
	{
		CRITICAL(m_cCritical);
		if(Peek()==0)return "";
		string ret = m_queueLines.front();
		m_queueLines.pop();

		if(m_queueLines.size()==0)m_bBackHalf = false;

		return ret;
	}
private:
	mutable CCriticalSection	m_cCritical;
	queue<string>				m_queueLines;
	bool						m_bBackHalf; //終端要素が改行で終わっていなかった場合はtrue
};

class CReadThread : public CThread2{
public:
	CReadThread(CPipe& cPipe)
	: m_cPipe(cPipe)
	{
	}
	int Run()
	{
		while(m_cPipe.IsValid()){
			std::string str;
			if(m_cPipe.ReadText(&str, true)){
				//デバッグ出力
				::OutputDebugStringA(str.c_str());
				//キューに格納
				m_cLineQueue.Push(str.c_str());
			}
			::Sleep(2);
		}
		return 0;
	}
	int Peek() const{ return m_cLineQueue.Peek(); }
	string Pop(){ return m_cLineQueue.Pop(); }
private:
	CPipe&		m_cPipe;
	CLineQueue	m_cLineQueue;
};



static void SendText(CPipe& cPipe, const char* szCommand, ...)
{
	//整形
	char buf[1024];
	va_list v;
	va_start(v, szCommand);
	vsprintf_s(buf, _countof(buf), szCommand, v);
	va_end(v);

	//送信
	DWORD dwWrote = 0;
	wprintf(L"<<send: %hs>>\n", cpp_chomp(buf).c_str()); fflush(stdout);
	BOOL bRet = ::WriteFile(cPipe.GetWriteHandle(), buf, (int)strlen(buf), &dwWrote, NULL);
	::FlushFileBuffers(cPipe.GetWriteHandle());
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// 初期化と終了
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
CFtp2::CFtp2()
: m_cPipeReadStdOut(1024*10)
, m_cPipeWriteStdIn(256)
, m_cProcess(
	L"c:\\cygwin\\bin\\ftp.exe -p",
	m_cPipeReadStdOut.GetWriteHandle(),
	m_cPipeReadStdOut.GetWriteHandle(),
	m_cPipeWriteStdIn.GetReadHandle()
)
{
	//読取スレッド作成
	m_pcThread = new CReadThread(m_cPipeReadStdOut);
}

CFtp2::~CFtp2()
{
	//先にキューを空にする
	while(m_pcThread->Peek())m_pcThread->Pop();

	//コマンドを送る
	SendText(m_cPipeWriteStdIn, "quit\n");

	//パイプを破棄する
	m_cPipeWriteStdIn._Dispose();
	m_cPipeReadStdOut._Dispose();

	//読取スレッドの終了待ち
	m_pcThread.WaitForDie();

	//プロセスの終了待ち
	::WaitForSingleObject(m_cProcess._GetHandle(), INFINITE);
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// 接続
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
void CFtp2::Connect(
	const char*	szHost,
	const char*	szUser,
	const char* szPass
)
{
	//先にキューを空にする
	while(m_pcThread->Peek())m_pcThread->Pop();

	//コマンドを送る
	SendText(m_cPipeWriteStdIn, "open %hs\n", szHost);
	SendText(m_cPipeWriteStdIn, "%hs\n", szUser);
	SendText(m_cPipeWriteStdIn, "%hs\n", szPass);

	//キューを空にする
	while(m_pcThread->Peek())m_pcThread->Pop();
}

void CFtp2::Disconnect()
{
	//先にキューを空にする
	while(m_pcThread->Peek())m_pcThread->Pop();

	//コマンドを送る
	SendText(m_cPipeWriteStdIn, "close\n");

	//「ftp>」が来るまで待つ
	bool bOk = false;
	while(1){
		if(m_pcThread->Peek()){
			string tmp = m_pcThread->Pop();
			if(strncmp(tmp.c_str(), "ftp>", 4)==0)break;
		}
		else{
			::Sleep(2);
		}
	}
}

bool CFtp2::IsConnected() const
{
	return true; //#######暫定
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// 移動インターフェース
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
bool CFtp2::ChDir(const char* dir)
{
	//先にキューを空にする
	while(m_pcThread->Peek())m_pcThread->Pop();
	
	//コマンドを送る
	SendText(m_cPipeWriteStdIn, "cd %hs\n", dir);

	//「ftp>」が来るまで待つ
	bool bOk = false;
	while(1){
		if(m_pcThread->Peek()){
			string tmp = m_pcThread->Pop();
			if(strncmp(tmp.c_str(), "250 ", 4)==0){ bOk = true; continue; }
			if(strncmp(tmp.c_str(), "ftp>", 4)==0)break;
		}
		else{
			::Sleep(2);
		}
	}
	
	//結果
	return bOk;
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// 取得インターフェース
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
std::vector<CFtpFileInfo> CFtp2::Ls()
{
	vector<CFtpFileInfo> ret;

	//先にキューを空にする
	while(m_pcThread->Peek())m_pcThread->Pop();
	
	//コマンドを送る
	SendText(m_cPipeWriteStdIn, "ls -la\n");
	
	//「ftp>」が来るまで待つ
	while(1){
		if(m_pcThread->Peek()){
			string tmp = m_pcThread->Pop();
			if(strncmp(tmp.c_str(), "Name (", 6)==0)continue;
			if(strncmp(tmp.c_str(), "ftp>", 4)==0)break;

			CFtpFileInfo cItem(tmp.c_str());
			if(strcmp(cItem.GetName(), ".")==0)continue;
			if(strcmp(cItem.GetName(), "..")==0)continue;
			ret.push_back(cItem);
		}
		else{
			::Sleep(2);
		}
	}
	
	//結果
	return ret;
}


void CFtp2::Download(const char* remote_file, const char* local_file)
{
	//先にキューを空にする
	while(m_pcThread->Peek())m_pcThread->Pop();

	//コマンドを送る //※local_file内の「\」は「\\」に直す必要がある
	char buf[256];	
	sprintf_s(buf, _countof(buf), "get \"%s\" \"%s\"\n", remote_file, cpp_strreplace(local_file,"\\","\\\\").c_str());
	SendText(m_cPipeWriteStdIn, buf);
	
	//「ftp>」が来るまで待つ
	while(1){
		if(m_pcThread->Peek()){
			string tmp = m_pcThread->Pop();
			if(strncmp(tmp.c_str(), "ftp>", 4)==0)break;
		}
		else{
			::Sleep(2);
		}
	}
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// 書込インターフェース
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void CFtp2::Upload(const char* local_file, const char* remote_file)
{
	//先にキューを空にする
	while(m_pcThread->Peek())m_pcThread->Pop();

	//コマンドを送る //※local_file内の「\」は「\\」に直す必要がある
	char buf[256];	
	sprintf_s(buf, _countof(buf),
		"put \"%s\" \"%s\"\n",
		cpp_strreplace(local_file,"\\","\\\\").c_str(),
		remote_file
	);
	SendText(m_cPipeWriteStdIn, buf);
	
	//「ftp>」が来るまで待つ
	while(1){
		if(m_pcThread->Peek()){
			string tmp = m_pcThread->Pop();
			if(strncmp(tmp.c_str(), "ftp>", 4)==0)break;
		}
		else{
			::Sleep(2);
		}
	}
}

void CFtp2::Rename(const char* old_name, const char* new_name)
{
	//先にキューを空にする
	while(m_pcThread->Peek())m_pcThread->Pop();

	//コマンドを送る //※local_file内の「\」は「\\」に直す必要がある
	char buf[256];	
	sprintf_s(buf, _countof(buf),
		"rename \"%s\" \"%s\"\n",
		old_name,
		new_name
	);
	SendText(m_cPipeWriteStdIn, buf);
	
	//「ftp>」が来るまで待つ
	while(1){
		if(m_pcThread->Peek()){
			string tmp = m_pcThread->Pop();
			if(strncmp(tmp.c_str(), "ftp>", 4)==0)break;
		}
		else{
			::Sleep(2);
		}
	}
}

void CFtp2::Delete(const char* remote_file)
{
	//先にキューを空にする
	while(m_pcThread->Peek())m_pcThread->Pop();

	//コマンドを送る //※local_file内の「\」は「\\」に直す必要がある
	char buf[256];	
	sprintf_s(buf, _countof(buf),
		"del \"%s\"\n",
		remote_file
	);
	SendText(m_cPipeWriteStdIn, buf);
	
	//「ftp>」が来るまで待つ
	while(1){
		if(m_pcThread->Peek()){
			string tmp = m_pcThread->Pop();
			if(strncmp(tmp.c_str(), "ftp>", 4)==0)break;
		}
		else{
			::Sleep(2);
		}
	}
}

void CFtp2::Mkdir(const char* new_dir)
{
	//先にキューを空にする
	while(m_pcThread->Peek())m_pcThread->Pop();

	//コマンドを送る //※local_file内の「\」は「\\」に直す必要がある
	char buf[256];	
	sprintf_s(buf, _countof(buf),
		"mkdir \"%s\"\n",
		new_dir
	);
	SendText(m_cPipeWriteStdIn, buf);
	
	//「ftp>」が来るまで待つ
	while(1){
		if(m_pcThread->Peek()){
			string tmp = m_pcThread->Pop();
			if(strncmp(tmp.c_str(), "ftp>", 4)==0)break;
		}
		else{
			::Sleep(2);
		}
	}
}





// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// 属性
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- パーミッション -- -- //
CFtpPermission::CFtpPermission(const char* szText)
: m_bIsDirectory(false)
{
	const char* p = szText;
	//最初の文字はディレクトリ属性
	if(*p++!='-')m_bIsDirectory=true;
	//各属性
	int nUser  = 0;
	int nGroup = 0;
	int nOther = 0;
	if(*p++!='-')nUser|=4;
	if(*p++!='-')nUser|=2;
	if(*p++!='-')nUser|=1;
	if(*p++!='-')nGroup|=4;
	if(*p++!='-')nGroup|=2;
	if(*p++!='-')nGroup|=1;
	if(*p++!='-')nOther|=4;
	if(*p++!='-')nOther|=2;
	if(*p++!='-')nOther|=1;
	//まとめる
	m_nPermission = (nUser<<6) | (nGroup<<3) | (nOther<<0);
}

int CFtpPermission::GetInt() const
{
	return m_nPermission;
}

std::string CFtpPermission::GetString() const
{
	static const char* table = "rwxrwxrwx";
	char ret[] = "---------";
	for(int i=0;i<9;i++){
		if((m_nPermission>>(8-i))&0x1){
			ret[i] = table[i];
		}
		else{
			ret[i] = '-';
		}
	}
	return ret;
}


// -- -- ファイル情報 -- -- //
CFtpFileInfo::CFtpFileInfo(const char* line)
{
	const char* p = line;
	int i = 0;
	string strDates[3];
	while(1){
		//空白読み飛ばし
		while(*p==' ')p++;
		if(!*p)break;
		//単語の終わりを探す
		const char* q = strchr(p, ' ');
		if(!q)q = strchr(p, '\0');
		//単語格納
		string tmp(p,q);
		switch(i){
		case 0: m_cPermission = CFtpPermission(tmp.c_str()); break;	//パーミッション
		case 1: break;												//ディレクトリに含まれるアイテム数
		case 2: m_strOwnerUser = tmp; break;						//所有ユーザ
		case 3: m_strOwnerGroup = tmp; break;						//所有ｸﾞﾙｰﾌﾟ
		case 4: m_nSize = _atoi64(tmp.c_str()); break;				//サイズ
		case 5: strDates[0] = tmp; break;							//月
		case 6: strDates[1] = tmp; break;							//日
		case 7: strDates[2] = tmp; break;							//年または時刻
		case 8: m_strName = tmp; break;								//名前
		}
		i++;
		if(i>=9)break;
		//次の単語
		p = q;
	}
	m_cTime = CFtpTime(strDates[0].c_str(), strDates[1].c_str(), strDates[2].c_str());
}

// -- -- 日時 -- -- //
CFtpTime::CFtpTime(const char* szMonth, const char* szDay, const char* szYearOrTime)
{
	//月の解釈
	static const char* aMonths[] = {
		"Jan", "Feb", "Mar", "Apr", "May", "Jun",
		"Jul", "Aug", "Sep", "Oct", "Nov", "Dec",
	};
	int nMonth = 0; //0:無効 1～12:有効
	for(int i=0;i<12;i++){
		::OutputDebugStringA("month:");
		::OutputDebugStringA(szMonth);
		::OutputDebugStringA("\r\n");
		if(_stricmp(aMonths[i], szMonth)==0){
			nMonth = i+1;
			break;
		}
		::OutputDebugStringA("/month\r\n");
	}
	if(nMonth==0)
		throw myexception("invalid month [%hs]", szMonth);

	//日の解釈
	int nDay = atoi(szDay);

	//年または時刻の解釈
	int nYear = 0;
	int nHour = 0;
	int nMin = 0;
	if(strchr(szYearOrTime, ':')){
		//時刻
		const char* p = szYearOrTime;
		const char* q = strchr(szYearOrTime, ':');
		const char* r = strchr(szYearOrTime, '\0');
		nHour = atoi(p);
		nMin = atoi(q+1);

		//年は今年
		{
			time_t _t = time(NULL);
			tm t = {0}; localtime_s(&t, &_t);
			nYear = t.tm_year + 1900;
		}
	}
	else{
		//年
		nYear = atoi(szYearOrTime);
	}

	//time_tに変換
	//nMonth, nDay, nYear, nHour, nMin -> m_time
	tm t = {0};
	t.tm_year = (nYear>=1900)?(nYear-1900):0;
	t.tm_mon =  nMonth-1;
	t.tm_mday = nDay;
	t.tm_hour = nHour;
	t.tm_min  = nMin;
	m_time = mktime(&t);
}

std::string CFtpTime::GetString() const
{
	tm t = {0};
	localtime_s(&t, &m_time);
	char buf[64];
	sprintf_s(
		buf,
		_countof(buf),
		"%04d/%02d/%02d %02d:%02d",
		t.tm_year + 1900,
		t.tm_mon + 1,
		t.tm_mday,
		t.tm_hour,
		t.tm_min
	);
	return buf;
}
