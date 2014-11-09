#include "common/_required.h"
#include "common/sys.h"
#include <DbLib.h>
#include <windows.h>
#include <vector>
#include <string>

int wmain(int argc, wchar_t* argv[])
{
	c2lib::Init(argc, argv);
	debugInit(DEBUG_OUT_DEBUGGER);

	CMySql sql;
	bool bOpen = sql.Open("192.168.7.100", "kobake", "sos-shock", "2chdb");
	if(1){
		// SELECTテスト //
		const char* szQuery = "select BOARD_NAME, unix_timestamp(LAST_COMMENT) from THREADS";
		int iQuery = sql.RunQuery(szQuery);
		std::vector< std::vector<std::wstring> > vRecords = sql.GetRows();
		for(int i = 0; i < (int)vRecords.size(); i++){
			for(int j = 0; j < (int)vRecords[i].size(); j++){
				printf("%ls, ", vRecords[i][j].c_str());
			}
			printf("\n");
		}
	}
	if(0){
		// INSERTテスト //
		const char* szQuery = "insert into THREADS(BOARD_NAME, LAST_COMMENT) values(?, from_unixtime(?))";
		int iQuery = sql.RunQuery(szQuery, L"あaaa", 100000);
		int test = iQuery;
	}

	return 0;
}

