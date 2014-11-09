#pragma once

/*
	�g�p��
		// �ݒ肻�̂P
		CLogType INFO;
		INFO.SetName("INFO");
		INFO.SetFile("hoge$yyyy$mm$dd$HH$MM$SS.log");
		INFO.SetConsole(1);

		// �ݒ肻�̂Q
		CLogManager::Instance()->Init();

		// �g�p
		INFO.Log("hoge%d", 10);
		KLOG(INFO, "hoge%d", 10);

	�o�͐�
		log/�`

	�o�͗�
		[2010/02/20 12:31:40][INFO][Abc.cpp:80:CHoge::Do] Hello
*/

#include "src/CLog.h"
#include "src/CLogType.h"
#include "src/CLogManager.h"
#include "src/Log.h"
