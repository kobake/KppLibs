#pragma once

#include <windows.h>

#pragma pack(push,1)
typedef struct{
	BYTE        bWidth;          // �C���[�W�̕��i�s�N�Z�����j
	BYTE        bHeight;         // �C���[�W�̍����i�s�N�Z�����j
	BYTE        bColorCount;     // �C���[�W�̐F���i256�F��葽���ꍇ��0�j
	BYTE        bReserved;       // Reserved ( must be 0)
	WORD        wPlanes;         // Color Planes
	WORD        wBitCount;       // ��s�N�Z��������̃r�b�g��
	DWORD       dwBytesInRes;    // �C���[�W�Ɏg�p���Ă���o�C�g��
	DWORD       dwImageOffset;   // �t�@�C�����̉��o�C�g�ڂ���L�^����Ă��邩�B
} ICONDIRENTRY, *LPICONDIRENTRY;

// �t�@�C���̐擪�ɑ��݂���B
// ���\�[�X�̌^�A�C���[�W�̐��AICONDIRENTRY�\���̂̔z���ێ����Ă���
typedef struct{
	WORD           idReserved;   // Reserved (must be 0)
	WORD           idType;       // ���\�[�X�̌^�@ICO�̏ꍇ��1
	WORD           idCount;      // �܂܂�Ă���C���[�W�̐�
	ICONDIRENTRY   idEntries[1]; // ICONDIRENTRY�\���̂̔z��
} ICONDIR, *LPICONDIR;

typedef struct{
	BITMAPINFOHEADER   icHeader;      // DIB�w�b�_
	RGBQUAD            icColors[1];   // �J���[�e�[�u��
	BYTE               icXOR[1];      // �C���[�W
	BYTE               icAND[1];      // �}�X�N
} ICONIMAGE, *LPICONIMAGE;

#pragma pack(pop)
