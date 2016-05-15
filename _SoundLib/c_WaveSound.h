#pragma once

#include <Windows.h>
#include <vector>
class App;


#include <windows.h>
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
	bool loadFile(const wchar *fpath);
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

