#pragma once

/*
	※注意：このヘッダは winsock2.h をインクルードします。
	これより前に winsock.h を含む windows.h をインクルードした場合はいろいろ問題が起こるので、
	プロジェクト側で _WINSOCKAPI_ を定義しておき、winsock.h の中身を解釈させないのが良い。
*/
#include "common/_required.h"
#include "src/CHost.h"
#include "src/CFtp.h"
#include "src/CUrl.h"
#include "src/CSocket.h"
#include "src/CInternet.h"
#include "src/CInternetSession.h"
#include "src/CHttpRequest.h"
#include "src/CHttp.h"

