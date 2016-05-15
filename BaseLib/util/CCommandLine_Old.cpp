#include "_required.h"
#include "CCommandLine_Old.h"
#include <DebugLib.h>
using namespace std;

inline bool is_sp(wchar_t c)
{
	return c==' ' || c=='\t';
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//               コンストラクタ・デストラクタ                  //
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
	//解析
	const wchar_t* p=str;
	const wchar_t* begin;
	const wchar_t* end;
	while(p){
		//空白読み飛ばし
		while(is_sp(*p))p++;

		//終端なら終了
		if(*p==L'\0')break;

		//トークン開始
		begin=p;

		//トークン
		if(*p=='"'){
			p++;
			while(*p!=L'"' && *p!=L'\0')p++;
			if(*p==L'"')p++;
		}
		else{
			while(!is_sp(*p) && *p!=L'\0')p++;
		}

		//トークン終了
		end=p;

		//クォーテーション除去
		if(*begin=='"')begin++;
		if(begin<end && *(end-1)=='"')end--;

		//リスト追加
		args.push_back(wstring(begin,end));
	}
	
	wstring s=str;
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           取得                              //
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
//                           解釈                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

inline bool isopt(wchar_t c)
{
	return c==L'-' || c==L'/';
}

//! 単純に、strが存在するかどうかを調べる。存在していればtrue。大文字、小文字は区別しない。
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

//! "-opt" または "/opt" を検索。大文字、小文字は区別しない
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
			//"="区切りの値
			if(arg[1+nopt]=='='){
				ret=&arg[1+nopt+1];
				found_i=i;
				found_n=1;
				goto lbl_ret;
			}
			//" "区切りの値
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
		//戻り値をstaticな領域に退避
		static wstring sret;
		sret=ret;
		//該当項目を除去
		for(int i=0;i<found_n;i++){
			remove(found_i);
		}
		//staticな領域を返す
		return sret.c_str();
	}else{
		return ret;
	}
}


//!オプション値の取得
/*!
"-opt=?" または "/opt=?" の "?" を取得。
"=" の代わりに " " も受け付ける。
"-opt" が存在しなければ def を返す。
"-opt" が存在するが、"?"が存在しない場合、"" を返す。
*/
const wchar_t* CCommandLine_Old::GetFlagValue(const wchar_t* szFlagName,const wchar_t* szDefaultValue) const
{
	return const_cast<CCommandLine_Old*>(this)->_getpopOption(szFlagName,szDefaultValue,false);
}

//!オプション値の取得。除去。
/*!
GetFlagValueの動作に加え、見つかった項目を取り除く。
*/
const wchar_t* CCommandLine_Old::PopFlag(const wchar_t* szFlagName,const wchar_t* szDefaultValue)
{
	return _getpopOption(szFlagName,szDefaultValue,true);
}



// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           編集                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//! コマンドラインバッファを書き換える
void CCommandLine_Old::Assign(size_t i,const wchar_t* str)
{
#if _MSC_VER >= 1400
	assert(i<args.size());
#endif
	args[i]=str;
}

//! 指定インデックスの項目を取り除く
void CCommandLine_Old::remove(size_t i)
{
#if _MSC_VER >= 1400
	assert(i<args.size());
#endif
	vector<wstring>::iterator p=args.begin();
	while(i>0){ i--; p++; }
	args.erase(p);
}

//! 最後尾に引数を追加する
void CCommandLine_Old::push_back(const wchar_t* str)
{
	args.push_back(str);
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           配列                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

const wchar_t* const* CCommandLine_Old::getArgs(size_t begin_index) const
{
	for(int i=0;i<(int)args.size();i++){
		args_buf[i]=args[i].c_str();
	}
	args_buf[args.size()]=NULL;
	return args_buf;
}
