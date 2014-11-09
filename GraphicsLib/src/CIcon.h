#pragma once

#include <windows.h>

#pragma pack(push,1)
typedef struct{
	BYTE        bWidth;          // イメージの幅（ピクセル数）
	BYTE        bHeight;         // イメージの高さ（ピクセル数）
	BYTE        bColorCount;     // イメージの色数（256色より多い場合は0）
	BYTE        bReserved;       // Reserved ( must be 0)
	WORD        wPlanes;         // Color Planes
	WORD        wBitCount;       // 一ピクセルあたりのビット数
	DWORD       dwBytesInRes;    // イメージに使用しているバイト数
	DWORD       dwImageOffset;   // ファイル中の何バイト目から記録されているか。
} ICONDIRENTRY, *LPICONDIRENTRY;

// ファイルの先頭に存在する。
// リソースの型、イメージの数、ICONDIRENTRY構造体の配列を保持している
typedef struct{
	WORD           idReserved;   // Reserved (must be 0)
	WORD           idType;       // リソースの型　ICOの場合は1
	WORD           idCount;      // 含まれているイメージの数
	ICONDIRENTRY   idEntries[1]; // ICONDIRENTRY構造体の配列
} ICONDIR, *LPICONDIR;

typedef struct{
	BITMAPINFOHEADER   icHeader;      // DIBヘッダ
	RGBQUAD            icColors[1];   // カラーテーブル
	BYTE               icXOR[1];      // イメージ
	BYTE               icAND[1];      // マスク
} ICONIMAGE, *LPICONIMAGE;

#pragma pack(pop)
