#include "c_WaveSound.h"
#include <Windows.h>

#include <BaseLib.h>
#include <IoLib.h>
#include <WinSystemLib.h>

#include <MMSystem.h>

//#include "c_App.h"

// http://www13.plala.or.jp/kymats/study/MULTIMEDIA/LoadWaveFile/LoadWaveFile.cpp
// PCM�`����WAVE�t�@�C����ǂݍ���
// char *lpFileName�F�t�@�C���̖��O���w���|�C���^
// WAVEFORMATEX *lpwfe�F�f�[�^���i�[����WAVEFORMATEX�\���̂��w���|�C���^
// WAVEHDR *lpwhdr�F�f�[�^���i�[����WAVEHDR�\���̂��w���|�C���^
// �߂�l�F0(����) or ���l(���s)


int LoadWaveMemory(const void* mem,WAVEFORMATEX *lpwfe,WAVEHDR *lpwhdr)
{
	BYTE* lpBuf = (BYTE*)mem;
	
	char str[4];
	strncpy(str,(char*)(lpBuf+8),4);
	if(strncmp(str,"WAVE",4)){
		HeapFree(GetProcessHeap(),0,lpBuf);
		MessageBox(NULL,L"WAVE�t�@�C�����w�肵�ĉ�����",L"error",MB_OK);
		return -2;
	}

	CopyMemory(lpwfe,lpBuf+20,16);		// �t�H�[�}�b�g�����R�s�[

	if(lpwfe->wFormatTag != WAVE_FORMAT_PCM){
		HeapFree(GetProcessHeap(),0,lpBuf);
		MessageBox(NULL,L"PCM�`����WAVE�t�@�C�����w�肵�ĉ�����",L"error",MB_OK);
		return -3;
	}

	strncpy(str,(char*)(lpBuf+36),4);
	int offset;
	if(!strncmp(str,"fact",4)) offset=56;		// fact�`�����N���L��
	else if(!strncmp(str,"data",4)) offset=44;	// data�`�����N(fact�`�����N������)
	else{
		HeapFree(GetProcessHeap(),0,lpBuf);
		MessageBox(NULL,L"���̕s���̃t�@�C���ł�",L"error",MB_OK);
		return -4;
	}

	// �g�`�f�[�^���R�s�[
	DWORD size=*(DWORD*)(lpBuf+offset-4);
	BYTE *lpWave=(BYTE*)HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,size);
	CopyMemory(lpWave,lpBuf+offset,size);

	lpwhdr->lpData=(LPSTR)lpWave;
	lpwhdr->dwBufferLength=size;
	lpwhdr->dwFlags=WHDR_BEGINLOOP | WHDR_ENDLOOP;
	lpwhdr->dwLoops=1;

	return 0;
}

// WAVE�t�@�C���̏I������
// HWAVEOUT hWaveOut�F�E�F�[�u�t�H�[���I�[�f�B�I�o�̓f�o�C�X�̃n���h��
// WAVEHDR *lpwhdr�F�N���[���A�b�v����f�[�^�u���b�N�����ʂ���WAVEHDR�\���̂��w���|�C���^

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
// -- -- -- -- -- -- -- -- -- -- -- --  �R���X�g���N�^�E�f�X�g���N�^  -- -- -- -- -- -- -- -- -- -- -- -- //
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
// -- -- -- -- -- -- -- -- -- -- -- -- �t�@�C�����o��  -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
bool WaveSound::loadFile(const wchar *fpath)
{
	dispose();

	// �t�@�C���I�[�v��
	HANDLE fh=CreateFile(fpath,GENERIC_READ,0,NULL,
		OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(fh==INVALID_HANDLE_VALUE){
		MessageBox(NULL,L"�t�@�C�����J���܂���",fpath,MB_OK);
		return false;
	}
	DWORD dwFileSize=GetFileSize(fh,NULL);
	m_buf.resize(dwFileSize);
	DWORD dwReadSize;
	ReadFile(fh,&m_buf[0],dwFileSize,&dwReadSize,NULL);
	CloseHandle(fh);

	// �ǂݍ���
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
// -- -- -- -- -- -- -- -- -- -- -- --  �Đ��E��~  -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
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


