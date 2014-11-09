#pragma once

#include "std/myexception.h"
#include "util/BitSet.h"

//!ID生成装置。ID_MIN～ID_MAXの範囲で、重複しないIDを生成する。
class CIdGenerator{
public:
	GEN_EXCEPTION(UniqueIdNotFound,L"CIdGenerator::UniqueIdNotFound");
public:
	static const int ID_MIN=0;
	static const int ID_MAX=32*1024-1;
	static const int ID_NUM=ID_MAX-ID_MIN+1;
public:
	//コンストラクタ・デストラクタ
	CIdGenerator();

	//インターフェース
	ulong GenerateID();
	void DisposeID(ulong id);

private:
	ulong cur_id;
	ulong use_cnt;
	BitSet<ID_NUM> table; //4KBytesバッファ
};


