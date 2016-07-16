#pragma once

#include <WinSock2.h>
#include <Windows.h>
#include <vector>
class App;


#include <stdio.h>
#include <mmsystem.h>
#include <amstream.h>


class WaveSound{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	WaveSound();
	~WaveSound();
	void dispose();
	//�t�@�C�����o��
	bool loadMemofy(const void* mem, int size);
	bool loadFile(const wchar_t *fpath);
	bool loadResource(App *app,int id);
	//�Đ��E��~
	void play();
	void stop();

public:
	static void globalVolume(float v);

private:
	std::vector<BYTE>	m_buf;
	WAVEHDR				m_whdr;
	HWAVEOUT			m_hWaveOut;
};

