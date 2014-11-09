//http://windowsmobile-dev.g.hatena.ne.jp/tmyt/20090801
#include "CIcon.h"
#include <windows.h>
#include <vector>
#include <algorithm>

// ICONDIRENTRYをソートするようオブジェクト
struct ICONDIR_SORT {
	bool operator()(ICONDIRENTRY lhs, ICONDIRENTRY rhs)
	{
		if(lhs.wBitCount == rhs.wBitCount){
			return lhs.bWidth > rhs.bWidth;
		}
		return lhs.wBitCount > rhs.wBitCount;
	}
};

// ICONDIRENTRYからいらないのを消すようオブジェクト
struct ICONDIR_ERASE_IF {
	WORD _;
	ICONDIR_ERASE_IF(WORD wBitCount) : _(wBitCount) {}
	bool operator()(ICONDIRENTRY lhs)
	{
		return lhs.wBitCount != _;
	}
};

/*!
 * \brief アイコンファイルを読み込みます。
 */
HICON loadIconFile(LPCTSTR lpFileName, BYTE bWidth, BYTE bHeight)
{
	HICON hIcon = NULL; //! 読み込んだアイコンのハンドル
	HANDLE hFile = CreateFile(lpFileName, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
	if(hFile != INVALID_HANDLE_VALUE){
		HANDLE hMapping = CreateFileMapping(hFile, 0, PAGE_READONLY, 0, 0, 0);
		if(hMapping != INVALID_HANDLE_VALUE){
			BYTE* ptr = (LPBYTE)MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, 0);
			if(ptr){
				LPICONDIR lpidr = (LPICONDIR)ptr;
				if(lpidr->idReserved == 0 && lpidr->idType == 1){
					LPICONDIRENTRY lpide = lpidr->idEntries;
					// 指定されたサイズのアイコンを探す。
					// まずはリストをソート
					std::vector<ICONDIRENTRY> list;
					HDC hDesktopDc = GetDC(NULL);
					WORD wBitCount = GetDeviceCaps(hDesktopDc, BITSPIXEL);
					for(int i = 0; i < lpidr->idCount; ++i){
						list.push_back(lpidr->idEntries[i]);
					}
					std::sort(list.begin(), list.end(), ICONDIR_SORT());
					// よさそうなのを探す
					lpide = &list[0];
					bool isBitCountMuched = false;
					// とりあえず画面の色数とあう色数のアイコンが含まれているか探す
					for(int i = 0; i < (int)list.size(); ++i){
						if(list[i].wBitCount == wBitCount){
							isBitCountMuched = true;
							break;
						}
					}
					// 良い感じの色数のアイコンがあるならそれ以外を消す
					if(isBitCountMuched){
//						list.erase(
							std::remove_if(
								list.begin(),
								list.end(),
								ICONDIR_ERASE_IF(wBitCount)
							);
//							list.end
//						);
					}
					// よさそうなサイズを探す
					for(int i = 0; i < (int)list.size(); ++i){
						if(list[i].bWidth == bWidth && list[i].bHeight == bHeight){
							lpide = &list[i];
							break;
						}
						lpide = &list[i];
					}
					// アイコンのデータを解析してデータを準備
					LPICONIMAGE lpiimg = (LPICONIMAGE)(ptr + lpide->dwImageOffset);
					BITMAPINFOHEADER bmhdr = lpiimg->icHeader;
					bmhdr.biHeight /= 2;
					int cCount = 0;
					switch (bmhdr.biBitCount){
					case 1: cCount = 2;   break;
					case 2: cCount = 4;   break;
					case 4: cCount = 16;  break;
					case 8: cCount = 256; break;
					}
					LPBITMAPINFO lpbminfXor = (LPBITMAPINFO)malloc(sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * cCount);
					LPBITMAPINFO lpbminfAnd = (LPBITMAPINFO)malloc(sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 2);
					DWORD xorStride  = (bmhdr.biWidth * bmhdr.biBitCount + 7) / 8 + 3 & ~ 3;
					DWORD andStride  = (bmhdr.biWidth + 7) / 8 + 3 & ~ 3;
					memcpy(lpbminfXor->bmiColors, lpiimg->icColors, sizeof(RGBQUAD) * cCount);
					LPBYTE lpbXOR = (LPBYTE)(lpiimg->icColors + cCount);
					LPBYTE lpbAND = lpbXOR + (xorStride * bmhdr.biHeight);
					lpbminfXor->bmiHeader = bmhdr;
					// デバイスコンテキスト作る
					HDC hDcXor = CreateCompatibleDC(hDesktopDc);
					// DIBセクションを作る
					BYTE *xorDIBits;
					HBITMAP hBmXor = CreateDIBSection(hDcXor, lpbminfXor, DIB_RGB_COLORS, (void**)&xorDIBits, NULL, 0);
					// DIBセクションにデータをセット
					memcpy(xorDIBits, lpbXOR, xorStride * bmhdr.biHeight);
					// マスク画像をDWORD境界からWORD境界へパックする
					DWORD pkAndStride  = (bmhdr.biWidth + 7) / 8 + 1 & ~ 1;
					LPBYTE pkAND = (LPBYTE)malloc(pkAndStride * bmhdr.biHeight);
					LPBYTE pkANDLast = pkAND + pkAndStride * bmhdr.biHeight;
					for(int i = 0; i < bmhdr.biHeight; ++i)memcpy(pkAND + pkAndStride * i, lpbAND + andStride * i, pkAndStride);
					for(int i = 0; i < bmhdr.biHeight>>1;++i){
						std::swap_ranges(pkAND + pkAndStride * i, pkAND + pkAndStride * i + pkAndStride, pkANDLast - pkAndStride * (i+1));
					}
					HBITMAP hBmAnd = CreateBitmap(bmhdr.biWidth, bmhdr.biHeight, 1, 1, pkAND);
					free(pkAND);
					// フルカラーなバッファにコピーする。さらに必要ならリサイズする
					lpbminfXor->bmiHeader.biWidth  = bWidth;
					lpbminfXor->bmiHeader.biHeight = bHeight;
					BYTE* xorDIBitsRes;
					HBITMAP hBmAndRes = CreateBitmap(bWidth, bHeight, 1, 1, NULL);
					HBITMAP hBmXorRes = CreateDIBSection(hDcXor, lpbminfXor, DIB_RGB_COLORS, (void**)&xorDIBitsRes, NULL, 0);
					HDC hdc1 = CreateCompatibleDC(hDesktopDc);
					HDC hdc2 = CreateCompatibleDC(hDesktopDc);
					HBITMAP hOldBmp1 = (HBITMAP)SelectObject(hdc1, hBmAnd);
					HBITMAP hOldBmp2 = (HBITMAP)SelectObject(hdc2, hBmAndRes);
					StretchBlt(hdc2, 0, 0, bWidth, bHeight, hdc1, 0, 0, lpide->bWidth, lpide->bHeight, SRCCOPY);
					SelectObject(hdc1, hOldBmp1);
					DeleteObject(hBmAnd);
					hBmAnd = hBmAndRes;
					SelectObject(hdc1, hBmXor);
					SelectObject(hdc2, hBmXorRes);
					StretchBlt(hdc2, 0, 0, bWidth, bHeight, hdc1, 0, 0, lpide->bWidth, lpide->bHeight, SRCCOPY);
					SelectObject(hdc1, hOldBmp1);
					DeleteObject(hBmXor);
					hBmXor = hBmXorRes;
					// 反転マスクを作って、XORマスクを正規化
					HBITMAP hBmAndInv = CreateBitmap(bWidth, bHeight, 1, 1, NULL);
					SelectObject(hdc1, hBmAnd);
					SelectObject(hdc2, hBmAndInv);
					RECT fill = {0, 0, bWidth, bHeight};
					FillRect(hdc2, &fill, (HBRUSH)GetStockObject(WHITE_BRUSH));
					BitBlt(hdc2, 0, 0, bWidth, bHeight, hdc1, 0, 0, SRCINVERT);
					SelectObject(hdc1, hBmXor);
					BitBlt(hdc1, 0, 0, bWidth, bHeight, hdc2, 0, 0, SRCAND);
					SelectObject(hdc1, hOldBmp1);
					SelectObject(hdc2, hOldBmp2);
					DeleteObject(hBmAndInv);
					DeleteDC(hdc1);
					DeleteDC(hdc2);
					// アイコンに変換
					ICONINFO iinf = {0};
					iinf.fIcon = TRUE;
					iinf.hbmColor = hBmXor;
					iinf.hbmMask  = hBmAnd;
					SetLastError(0);
					hIcon = CreateIconIndirect(&iinf);
					// リソースの解放
					free(lpbminfXor);
					free(lpbminfAnd);
					DeleteObject(hBmXor);
					DeleteObject(hBmAnd);
					DeleteDC(hDcXor);
					ReleaseDC(NULL, hDesktopDc);
				}
				UnmapViewOfFile(ptr);
			}
			CloseHandle(hMapping);
		}
		CloseHandle(hFile);
	}
	return hIcon;
}
