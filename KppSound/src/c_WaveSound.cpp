#include "c_WaveSound.h"
#include <Windows.h>

#include <BaseLib.h>
#include <IoLib.h>
#include <WinSystemLib.h>

#include <MMSystem.h>

//#include "c_App.h"

// http://www13.plala.or.jp/kymats/study/MULTIMEDIA/LoadWaveFile/LoadWaveFile.cpp
// PCM形式のWAVEファイルを読み込む
// char *lpFileName：ファイルの名前を指すポインタ
// WAVEFORMATEX *lpwfe：データを格納するWAVEFORMATEX構造体を指すポインタ
// WAVEHDR *lpwhdr：データを格納するWAVEHDR構造体を指すポインタ
// 戻り値：0(成功) or 負値(失敗)


int LoadWaveMemory(const void* mem,WAVEFORMATEX *lpwfe,WAVEHDR *lpwhdr)
{
	BYTE* lpBuf = (BYTE*)mem;
	
	char str[4];
	strncpy(str,(char*)(lpBuf+8),4);
	if(strncmp(str,"WAVE",4)){
		HeapFree(GetProcessHeap(),0,lpBuf);
		MessageBox(NULL,L"WAVEファイルを指定して下さい",L"error",MB_OK);
		return -2;
	}

	CopyMemory(lpwfe,lpBuf+20,16);		// フォーマット情報をコピー

	if(lpwfe->wFormatTag != WAVE_FORMAT_PCM){
		HeapFree(GetProcessHeap(),0,lpBuf);
		MessageBox(NULL,L"PCM形式のWAVEファイルを指定して下さい",L"error",MB_OK);
		return -3;
	}

	strncpy(str,(char*)(lpBuf+36),4);
	int offset;
	if(!strncmp(str,"fact",4)) offset=56;		// factチャンクが有る
	else if(!strncmp(str,"data",4)) offset=44;	// dataチャンク(factチャンクが無い)
	else{
		HeapFree(GetProcessHeap(),0,lpBuf);
		MessageBox(NULL,L"正体不明のファイルです",L"error",MB_OK);
		return -4;
	}

	// 波形データをコピー
	DWORD size=*(DWORD*)(lpBuf+offset-4);
	BYTE *lpWave=(BYTE*)HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,size);
	CopyMemory(lpWave,lpBuf+offset,size);

	lpwhdr->lpData=(LPSTR)lpWave;
	lpwhdr->dwBufferLength=size;
	lpwhdr->dwFlags=WHDR_BEGINLOOP | WHDR_ENDLOOP;
	lpwhdr->dwLoops=1;

	return 0;
}

// WAVEファイルの終了処理
// HWAVEOUT hWaveOut：ウェーブフォームオーディオ出力デバイスのハンドル
// WAVEHDR *lpwhdr：クリーンアップするデータブロックを識別するWAVEHDR構造体を指すポインタ

void CloseWaveFile(HWAVEOUT hWaveOut,WAVEHDR *lpwhdr)
{
	waveOutReset(hWaveOut);
	waveOutUnprepareHeader(hWaveOut,lpwhdr,sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
	if(lpwhdr->lpData){
		HeapFree(GetProcessHeap(),0,lpwhdr->lpData);
		lpwhdr->lpData=NULL;
	};
}
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- --  コンストラクタ・デストラクタ  -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

WaveSound::WaveSound()
{
	m_hWaveOut = NULL;
}

WaveSound::~WaveSound()
{
	dispose();
}

void WaveSound::dispose()
{
    if (m_hWaveOut)
    {
		waveOutClose(m_hWaveOut);
		m_hWaveOut = NULL;
    }
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- ファイル入出力  -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
bool WaveSound::loadFile(const wchar *fpath)
{
	dispose();

	// ファイルオープン
	HANDLE fh=CreateFile(fpath,GENERIC_READ,0,NULL,
		OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(fh==INVALID_HANDLE_VALUE){
		MessageBox(NULL,L"ファイルを開けません",fpath,MB_OK);
		return false;
	}
	DWORD dwFileSize=GetFileSize(fh,NULL);
	m_buf.resize(dwFileSize);
	DWORD dwReadSize;
	ReadFile(fh,&m_buf[0],dwFileSize,&dwReadSize,NULL);
	CloseHandle(fh);

	// 読み込み
	WAVEFORMATEX wfe;
	LoadWaveMemory(&m_buf[0], &wfe, &m_whdr);
	waveOutOpen(&m_hWaveOut,WAVE_MAPPER,&wfe,0,0,CALLBACK_NULL);
	waveOutPrepareHeader(m_hWaveOut,&m_whdr,sizeof(WAVEHDR));

	return true;
}
bool WaveSound::loadResource(App *app,int id)
{
	dispose();
	//
	app->loadResource2(&m_buf, id, WAVE);
	WAVEFORMATEX wfe;
	LoadWaveMemory(&m_buf[0], &wfe, &m_whdr);
	waveOutOpen(&m_hWaveOut,WAVE_MAPPER,&wfe,0,0,CALLBACK_NULL);
	waveOutPrepareHeader(m_hWaveOut,&m_whdr,sizeof(WAVEHDR));

	if(m_buf.size()){
		return true;
	}else{
		return false;
	}
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- --  再生・停止  -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
float g_volume = 1;
void WaveSound::globalVolume(float v)
{
	g_volume = v;
}

void WaveSound::play()
{
	DWORD dw = MAKELONG(0xFFFF * g_volume, 0xFFFF * g_volume);
	waveOutSetVolume(m_hWaveOut, dw);
	waveOutWrite(m_hWaveOut,&m_whdr,sizeof(WAVEHDR));
}

void WaveSound::stop()
{
	::waveOutReset(m_hWaveOut);
}


