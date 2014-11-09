#pragma once

#include <vector>
#include <string>

//!コマンドライン解析クラス
class CCommandLine_Old{
public:
	//コンストラクタ・デストラクタ
	CCommandLine_Old(const wchar_t* str);
	CCommandLine_Old(int argc,const wchar_t* argv[]);
	CCommandLine_Old(int argc,wchar_t* argv[]);

	//取得
	size_t size() const;                       //!< 引数の数
	const wchar_t* at(size_t i) const;           //!< 引数を取得
	const wchar_t* operator [] (size_t i) const; //!< 引数を取得

	//main式 取得
	size_t argc() const{ return size(); }
	const wchar_t** argv() const{ return (const wchar_t**)getArgs(0); } 

	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
	//                           解釈                              //
	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

	bool existRaw(const wchar_t* str) const;          //!< 単純に、strが存在するかどうかを調べる。存在していればtrue。大文字、小文字は区別しない。
	bool ExistFlag(const wchar_t* opt) const;       //!< "-opt" または "/opt" を検索。大文字、小文字は区別しない
	int FindFlag(const wchar_t* opt) const;         //!< "-opt" または "/opt" を検索。大文字、小文字は区別しない

	const wchar_t* _getpopOption(const wchar_t* opt,const wchar_t* def,bool to_remove);

	/*
		!オプション値の取得
		"-opt=?" または "/opt=?" の "?" を取得。
		"=" の代わりに " " も受け付ける。
		"-opt" が存在しなければ NULL を返す。
		"-opt" が存在するが、"?"が存在しない場合、"" を返す。
	*/
	const wchar_t* GetFlagValue(const wchar_t* szFlagName,const wchar_t* szDefaultValue) const;

	/*!
		オプション値の取得。除去。
		GetFlagValueの動作に加え、見つかった項目を取り除く。
	*/
	const wchar_t* PopFlag(const wchar_t* szFlagName,const wchar_t* szDefaultValue);

	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
	//                           編集                              //
	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
	void Assign(size_t i,const wchar_t* str);         //!< コマンドラインバッファを書き換える
	void remove(size_t i);                          //!< 指定インデックスの項目を取り除く
	void push_back(const wchar_t* str);               //!< 最後尾に引数を追加する

	//配列
	const wchar_t* const* getArgs(size_t begin_index) const; //!< 終端が NULL の配列を返す
private:
	std::vector<std::wstring> args;
	mutable const wchar_t* args_buf[64];
};
