#include "_required.h"
#include "CIdGenerator.h"

//IdCollection


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//               �R���X�g���N�^�E�f�X�g���N�^                  //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

CIdGenerator::CIdGenerator()
{
	cur_id=ID_MIN;
	use_cnt=0;
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                     �C���^�[�t�F�[�X                        //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

ulong CIdGenerator::GenerateID()
{
	if(use_cnt>=ID_NUM){ throw UniqueIdNotFound(); }

	//cur_id����T��
	ulong i=cur_id;
	while(1){
		if(!table.get(i-ID_MIN))break;
		i++;
		if(i>ID_MAX)i=ID_MIN;
	}

	//cur_id��i�߂�
	cur_id=i+1;
	if(cur_id>ID_MAX)cur_id=ID_MIN;

	//�t���O�𗧂Ă�
	table.store(i-ID_MIN);

	return i;
}

void CIdGenerator::DisposeID(ulong id)
{
	table.clear(id-ID_MIN);
	cur_id=id;
}

