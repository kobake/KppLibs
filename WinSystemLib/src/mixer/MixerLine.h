#pragma once

#define MIXERLINE_DESTINATION	0
#define MIXERLINE_SOURCE		1

#include "MixerControl.h"


class MixerLine{
public:
	enum ControlType{
		VOLUME=1,
		MUTE,
	};
public:
	//コンストラクタ・デストラクタ
	MixerLine(HMIXER _hMixer,MIXERLINE* _mxl);
	virtual ~MixerLine();
	//基本情報の取得
	const wchar_t* getName();
	int getConnectionNum();
	int getControlNum();
	DWORD getLineID();
	DWORD getComponentType();
	MMRESULT getLastError();
	//コントロールの取得
	MixerControl** getControls();
	MixerControl* createControl(ControlType type);
	//入力系統の取得
	MixerLine* getSourceLine(int src_index);
private:
	MMRESULT last_error;
	HMIXER hMixer;
	MIXERLINE mxl;
	MixerControl** ret_MixerControl;
};

