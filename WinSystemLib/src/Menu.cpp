#include "_required.h"
#include "Menu.h"


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//               �R���X�g���N�^�E�f�X�g���N�^                  //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

Menu::Menu(HINSTANCE hInst,int id)
{
	hMenu=LoadMenu(hInst,MAKEINTRESOURCE(id));
}

Menu::~Menu()
{
	DestroyMenu(hMenu);
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                     �C���^�[�t�F�[�X                        //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void Menu::popup(HWND hwnd)
{
	//�T�u���j���[���擾
	HMENU hSubMenu=GetSubMenu(hMenu,0);

	//�J�[�\���ʒu�擾
	POINT pt;
	GetCursorPos(&pt);

	//���j���[�\��
	TrackPopupMenu(
		hSubMenu,
		TPM_BOTTOMALIGN, //���j���[���[����Ƃ���
		pt.x,pt.y,       //���W
		0,               //�\��ς�
		hwnd,            //�I�[�i�[
		NULL             //���������
	);
}

