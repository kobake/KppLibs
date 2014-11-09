#pragma once

namespace util{

//ディレクトリ操作
bool dexist(const wchar_t* path);
int dtouch(const wchar_t* _path);

//属性の取得
__int64 getfsize(const wchar_t* szPath);
int getfpsize(FILE *fp);

//ファイル単位の操作
bool fexist(const wchar_t* path);
int ftouch(const wchar_t* path);
int fcopy(FILE *fpin,FILE *fpout);            //!<ストリームからストリームへコピーを行う
int fcopy(const wchar_t* from,const wchar_t* to); //!<ファイルからファイルへコピーを行う
void fbackup(const wchar_t* fpath); //!<ファイルのバックアップを作成する。バックアップファイル名は「(元のファイル名).(インデックス)」とする。

//内容の編集
int fstrreplace(FILE *fpin,FILE *fpout,const wchar_t* from,const wchar_t* to);
int fstrreplace(const wchar_t* path,const wchar_t* from,const wchar_t* to);
int fstrreplace(const wchar_t* pathfrom,const wchar_t* pathto,const wchar_t* from,const wchar_t* to);

//メモリ変換
void* file_alloc(const wchar_t* path,const wchar_t* mode);
void file_free(void* p);
/*
inline int fstrreplace(FILE *fpin,FILE *fpout,const string& from,const string& to)
{
	return fstrreplace(fpin,fpout,from.c_str(),to.c_str());
}
inline int fstrreplace(const string& path,const string& from,const string& to)
{
	return fstrreplace(path.c_str(),from.c_str(),to.c_str());
}
inline int fstrreplace(const string& pathfrom,const string& pathto,const string& from,const string& to)
{
	return fstrreplace(pathfrom.c_str(),pathto.c_str(),from.c_str(),to.c_str());
}
*/

} //namespace util

using namespace util;
