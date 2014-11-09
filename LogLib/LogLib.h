#pragma once

/*
	使用例
		// 設定その１
		CLogType INFO;
		INFO.SetName("INFO");
		INFO.SetFile("hoge$yyyy$mm$dd$HH$MM$SS.log");
		INFO.SetConsole(1);

		// 設定その２
		CLogManager::Instance()->Init();

		// 使用
		INFO.Log("hoge%d", 10);
		KLOG(INFO, "hoge%d", 10);

	出力先
		log/～

	出力例
		[2010/02/20 12:31:40][INFO][Abc.cpp:80:CHoge::Do] Hello
*/

#include "src/CLog.h"
#include "src/CLogType.h"
#include "src/CLogManager.h"
#include "src/Log.h"
