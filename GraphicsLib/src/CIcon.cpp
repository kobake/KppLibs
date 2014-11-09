//http://windowsmobile-dev.g.hatena.ne.jp/tmyt/20090801
#include "CIcon.h"
#include <windows.h>
#include <vector>
#include <algorithm>

// ICONDIRENTRY���\�[�g����悤�I�u�W�F�N�g
struct ICONDIR_SORT {
	bool operator()(ICONDIRENTRY lhs, ICONDIRENTRY rhs)
	{
		if(lhs.wBitCount == rhs.wBitCount){
			return lhs.bWidth > rhs.bWidth;
		}
		return lhs.wBitCount > rhs.wBitCount;
	}
};

// ICONDIRENTRY���炢��Ȃ��̂������悤�I�u�W�F�N�g
struct ICONDIR_ERASE_IF {
	WORD _;
	ICONDIR_ERASE_IF(WORD wBitCount) : _(wBitCount) {}
	bool operator()(ICONDIRENTRY lhs)
	{
		return lhs.wBitCount != _;
	}
};

/*!
 * \brief �A�C�R���t�@�C����ǂݍ��݂܂��B
 */
HICON loadIconFile(LPCTSTR lpFileName, BYTE bWidth, BYTE bHeight)
{
	HICON hIcon = NULL; //! �ǂݍ��񂾃A�C�R���̃n���h��
	HANDLE hFile = CreateFile(lpFileName, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
	if(hFile != INVALID_HANDLE_VALUE){
		HANDLE hMapping = CreateFileMapping(hFile, 0, PAGE_READONLY, 0, 0, 0);
		if(hMapping != INVALID_HANDLE_VALUE){
			BYTE* ptr = (LPBYTE)MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, 0);
			if(ptr){
				LPICONDIR lpidr = (LPICONDIR)ptr;
				if(lpidr->idReserved == 0 && lpidr->idType == 1){
					LPICONDIRENTRY lpide = lpidr->idEntries;
					// �w�肳�ꂽ�T�C�Y�̃A�C�R����T���B
					// �܂��̓��X�g���\�[�g
					std::vector<ICONDIRENTRY> list;
					HDC hDesktopDc = GetDC(NULL);
					WORD wBitCount = GetDeviceCaps(hDesktopDc, BITSPIXEL);
					for(int i = 0; i < lpidr->idCount; ++i){
						list.push_back(lpidr->idEntries[i]);
					}
					std::sort(list.begin(), list.end(), ICONDIR_SORT());
					// �悳�����Ȃ̂�T��
					lpide = &list[0];
					bool isBitCountMuched = false;
					// �Ƃ肠������ʂ̐F���Ƃ����F���̃A�C�R�����܂܂�Ă��邩�T��
					for(int i = 0; i < (int)list.size(); ++i){
						if(list[i].wBitCount == wBitCount){
							isBitCountMuched = true;
							break;
						}
					}
					// �ǂ������̐F���̃A�C�R��������Ȃ炻��ȊO������
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
					// �悳�����ȃT�C�Y��T��
					for(int i = 0; i < (int)list.size(); ++i){
						if(list[i].bWidth == bWidth && list[i].bHeight == bHeight){
							lpide = &list[i];
							break;
						}
						lpide = &list[i];
					}
					// �A�C�R���̃f�[�^����͂��ăf�[�^������
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
					// �f�o�C�X�R���e�L�X�g���
					HDC hDcXor = CreateCompatibleDC(hDesktopDc);
					// DIB�Z�N�V���������
					BYTE *xorDIBits;
					HBITMAP hBmXor = CreateDIBSection(hDcXor, lpbminfXor, DIB_RGB_COLORS, (void**)&xorDIBits, NULL, 0);
					// DIB�Z�N�V�����Ƀf�[�^���Z�b�g
					memcpy(xorDIBits, lpbXOR, xorStride * bmhdr.biHeight);
					// �}�X�N�摜��DWORD���E����WORD���E�փp�b�N����
					DWORD pkAndStride  = (bmhdr.biWidth + 7) / 8 + 1 & ~ 1;
					LPBYTE pkAND = (LPBYTE)malloc(pkAndStride * bmhdr.biHeight);
					LPBYTE pkANDLast = pkAND + pkAndStride * bmhdr.biHeight;
					for(int i = 0; i < bmhdr.biHeight; ++i)memcpy(pkAND + pkAndStride * i, lpbAND + andStride * i, pkAndStride);
					for(int i = 0; i < bmhdr.biHeight>>1;++i){
						std::swap_ranges(pkAND + pkAndStride * i, pkAND + pkAndStride * i + pkAndStride, pkANDLast - pkAndStride * (i+1));
					}
					HBITMAP hBmAnd = CreateBitmap(bmhdr.biWidth, bmhdr.biHeight, 1, 1, pkAND);
					free(pkAND);
					// �t���J���[�ȃo�b�t�@�ɃR�s�[����B����ɕK�v�Ȃ烊�T�C�Y����
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
					// ���]�}�X�N������āAXOR�}�X�N�𐳋K��
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
					// �A�C�R���ɕϊ�
					ICONINFO iinf = {0};
					iinf.fIcon = TRUE;
					iinf.hbmColor = hBmXor;
					iinf.hbmMask  = hBmAnd;
					SetLastError(0);
					hIcon = CreateIconIndirect(&iinf);
					// ���\�[�X�̉��
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
