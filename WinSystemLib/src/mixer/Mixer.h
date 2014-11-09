#pragma once

#include "MixerLine.h"

class MixerInfo{
public:
	static int getDeviceNum()
	{
		return mixerGetNumDevs();
	}
};

/*
MIXERLINE_COMPONENTTYPE_DST_SPEAKERS
MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT
MIXERLINE_COMPONENTTYPE_SRC_SYNTHESIZER
MIXERLINE_COMPONENTTYPE_SRC_ANALOG (複数のﾗｲﾝがある場合がある)
MIXERLINE_COMPONENTTYPE_SRC_COMPACTDISC
MIXERLINE_COMPONENTTYPE_SRC_LINE
MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE
MIXERLINE_COMPONENTTYPE_SRC_TELEPHONE
MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT (PCスピーカ)
*/

#include <windef.h> //HWND

class Mixer{
public:
	enum Type{
		SPEAKER=1,
		WAVEOUT,
		LINEIN,
	};
public:
	//コンストラクタ・デストラクタ
	Mixer(int mixer_id,HWND hwndCallback=NULL);
	virtual ~Mixer();
	//オープン・クローズ
	void Open(int mixer_id,HWND hwndCallback=NULL);
	void Close();
	//基本情報の取得
	const wchar_t* getName();
	//出力ライン
	int getDestinationCount();
	MixerLine* createDestinationLine(int dest_index);
	MixerLine* createDestinationLineByType(Type _type);
	//入力ライン
	MixerLine* createSourceLine(int dest_index,int src_index);
	MixerLine* createSourceLineByType(Type _type);
	//ミキサーコントロール
	MixerControl* createControlByID(DWORD ctl_id);
	//実装アクセス
	HMIXER _getHMIXER() const{ return hMixer; }
protected:
	//実装補助
	MixerLine* _createLine(int dest_index,int src_index,int type);
	MixerLine* _createLineByType(DWORD type);
private:
	MMRESULT last_error;
	HMIXER hMixer;
	MIXERCAPS mcaps;
	//
	MixerLine* dst_lines[16];
	MixerLine* src_lines[16];
};

