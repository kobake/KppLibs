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
	//コンストラクタ・デストラクタ
	WaveSound();
	~WaveSound();
	void dispose();
	//ファイル入出力
	bool loadMemofy(const void* mem, int size);
	bool loadFile(const wchar *fpath);
	bool loadResource(App *app,int id);
	//再生・停止
	void play();
	void stop();

public:
	static void globalVolume(float v);

private:
	std::vector<BYTE>	m_buf;
	WAVEHDR				m_whdr;
	HWAVEOUT			m_hWaveOut;
};

