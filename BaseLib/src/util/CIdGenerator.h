#pragma once

#include "std/myexception.h"
#include "util/BitSet.h"

//!ID�������u�BID_MIN�`ID_MAX�͈̔͂ŁA�d�����Ȃ�ID�𐶐�����B
class CIdGenerator{
public:
	GEN_EXCEPTION(UniqueIdNotFound,L"CIdGenerator::UniqueIdNotFound");
public:
	static const int ID_MIN=0;
	static const int ID_MAX=32*1024-1;
	static const int ID_NUM=ID_MAX-ID_MIN+1;
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CIdGenerator();

	//�C���^�[�t�F�[�X
	ulong GenerateID();
	void DisposeID(ulong id);

private:
	ulong cur_id;
	ulong use_cnt;
	BitSet<ID_NUM> table; //4KBytes�o�b�t�@
};


