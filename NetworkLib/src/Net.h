#pragma once

//!< Win32ソケットシステム
class Net{
public:
	static void init();       //!< DLL初期化。基本的にはc2libが自動で呼び出す。
	static void print_info(); //!< 情報をコンソールに出力する。
};
